// Standard C Libraries
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>
#include <vector>
#include <string>

// Server Libraries
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Apriltags libraries
#include "apriltags/apriltag.h"
#include "apriltags/common/image_u8.h"
#include "apriltags/tag36h11.h"
#include "apriltags/tag36h10.h"
#include "apriltags/tag36artoolkit.h"
#include "apriltags/tag25h9.h"
#include "apriltags/tag25h7.h"

#include "apriltags/common/zarray.h"
#include "apriltags/common/getopt.h"

// Our extensions for chromatags
#include "lib/rgb2lab.hpp" // functions to convert to rgb to lab, and seperate color channels
#include "lib/pnm2mat.hpp" // functions to convert pnm to and from mat

#define MY_PORT		"9499"

/*
 * Generates the object points based on the size of the chromatag
 */
std::vector<cv::Point3f> generateObjectPoints(float tagSize){
  std::vector<cv::Point3f> points;
  // Z of zero may not work properly
  points.push_back(cv::Point3f(0.0, 0.0, 0.0));
  points.push_back(cv::Point3f(0.0, -tagSize, 0.0));
  points.push_back(cv::Point3f(-tagSize, -tagSize, 0.0));
  points.push_back(cv::Point3f(-tagSize, 0.0,  0.0));

  return points;
}

/**
 * getCameraMatrix():
 *      Returns the camera matrix (OpenCV Mat)
 *      Obtained by running the camera calibration program
 *      User has to input the values manually
 */
Mat getCameraMatrix(){

  //1016.37 0.0 639.5
  //0.0 1016.37 359.5
  //0.0 0.0 1.0

  cv::Mat cameraMatrix(3,3,cv::DataType<double>::type);

  cameraMatrix.at<double>(0,0) = 1016.37;
  cameraMatrix.at<double>(1,0) = 0.0;
  cameraMatrix.at<double>(2,0) = 639.5;

  cameraMatrix.at<double>(0,1) = 0.0;
  cameraMatrix.at<double>(1,1) = 1016.37;
  cameraMatrix.at<double>(2,1) = 1359.5;

  cameraMatrix.at<double>(0,2) = 0.0;
  cameraMatrix.at<double>(1,2) = 0.0;
  cameraMatrix.at<double>(2,2) = 1.0;

  return cameraMatrix;
}

/**
 * getCameraDist(): 
 *      Returns the distortion matrix (OpenCV Mat)
 *      These values are inputed manually by the user, 
 *      using the camera calibration programs.
 */
Mat getCameraDist(){

  // 0.0010889 0.2442992 0.0 0.0 -0.6546799
  std::vector<double> distCoeffs;
  distCoeffs.push_back(0.0010889);
  distCoeffs.push_back(0.2442992);
  distCoeffs.push_back(0.0);
  distCoeffs.push_back(0.0);
  distCoeffs.push_back(-0.6546799);

  return Mat(distCoeffs);
}

/**
 * handle_session(int session_fd):
 *     Runs when a user connects to the server
 *     Connection port (MY_PORT): 9499
 *     
 *     Sends the client an RT matrix, in the form:
 *
 *     [R00,R10,R20,T0,R01,R11,R21,T1,R02,R12,R22,T2]\n
 *
 *     The chromaTag size is assumed by be 3cm, 
 *     the function generateObjectTag(size) is set to 3cm.
 */
void handle_session(int session_fd){

  int MAXBUF = 2048;

  bool showGradientEdges = false;
  bool found = false;
  bool init = true;
  vector<Point2f> initPts;
  
  VideoCapture cap(0);                                      // open the default camera
  
  int w = 768;
  int h = 432;

  Size size(w,h);                                           // size of desired frame: 1280x720, 768x432
  if(!cap.isOpened())                                       // check if camera opened
    return;

  Mat a, b, g, frame, src;

  /* From apriltag_demo.c */

  int maxiters = 5;
  const int hamm_hist_max = 10;
  int quiet = 0;

  apriltag_family_t *tf = tag36h11_create();                // Apriltag family 36h11, can change
  tf->black_border = 1;                                     // Set tag family border size

  apriltag_detector_t *td = apriltag_detector_create();     // Apriltag detector
  apriltag_detector_add_family(td, tf);                     // Add apriltag family

  td->quad_decimate = 1.0;                                  // Decimate input image by factor
  td->quad_sigma = 0.0;                                     // No blur (I think)
  td->nthreads = 4;                                         // 4 treads provided
  td->debug = 0;                                            // No debuging output
  td->refine_decode = 0;                                    // Don't refine decode
  td->refine_pose = 0;                                      // Don't refine pose

  // Output variables
  char imgSize[20];
  char renderTime[20];
  char detectString[50];
  char convertTime[50];
  char displayString[120];
  char outputString[120];
  char locationString[120];
  double time_taken = 0.0;

  long double totalFPS = 0.0;
  double count = 0.0;

  char buffer[MAXBUF]; // buffer to transfer over server

  /* End of apriltag_demo.c */

  std::vector<cv::Point3f> objPts = generateObjectPoints(3);
  Mat cameraMatrix = getCameraMatrix();
  Mat distCoeffs = getCameraDist();
  Mat rvec, tvec;
  double centerPoint[2];

  int opt1 = 0;
  int opt2 = 0;					
  int searchWidth = 0;
  int searchHeight = 0;
  vector<Point2f> pts;

  while(1){

    memset(buffer, 0, sizeof(buffer));

    clock_t t;
    t = clock();
    
    cap >> src;                                               // Get a new frame from camera
    
    if(found){
      w = 3*src.size().width/5;                              // Reset width based on frame
      h = 3*src.size().height/5;                             // Reset height based on frame
      size = Size(w,h);                                      // Sets size based on width and height
      resize(src,frame,size);                                // Resize to smaller image if tag found

      opt1 = centerPoint[0];                                 // Rectangle to search on prediction
      opt2 = centerPoint[1];                                 // Rectangle to search on prediction
      searchWidth = w/2;                                     // Search predicted location, width
      searchHeight = h/2;                                      // Search predicted location, height
      opt1 = opt1 - searchWidth/2;                           // Rectangle to search on prediction
      opt2 = opt2 - searchHeight/2;                          // Rectangle to search on prediction
      
      std::cout << opt1 << ", " << opt2 << std::endl;
      
      // Ensures search rectangle is within the search frame
      if((opt1 + searchWidth < frame.size().width && opt1 > 0)
      && (opt2 + searchHeight < frame.size().height && opt2 > 0)){
        Mat newFrame = frame;
        frame = newFrame(Rect(opt1,opt2,searchWidth,searchHeight));
      }else{
        std::cout << "\n\nNO SEARCH FRAME" << std::endl;
        opt1 = opt2 = 0;
        searchWidth = searchHeight = 1;
      }
    }else{
      frame = src;                                            // Keep standard image if no tag
      opt1 = opt2 = 0;
      searchWidth = searchHeight = 1;
    }
    
    //frame = RGB2YUV(frame);                                 // Just for comparison
    frame = RGB2LAB(frame);                                   // Returns lab space
    frame = alphaLAB(frame);                                  // Look at only a channel
    
    //src = frame;
    std::cout << "Center: (" << centerPoint[0] << ", " << centerPoint[1] << ") ";
    std::cout << " Size: " << frame.size().width <<"x" << frame.size().height << std::endl;

    if(showGradientEdges){
      src = gradientEdges(src);                               // Show graprintfnt for fun
    }
    
    // determine time to convert
    time_taken = ((double)(clock() - t))/(CLOCKS_PER_SEC/1000);
    sprintf(convertTime, "Convert Time: %5.3fms", time_taken);

    pnm_t *pnm = mat2pnm(&frame);
    image_u8_t *im = pnm_to_image_u8(pnm);                    // Convert pnm to gray image_u8
    if (im == NULL) {                                         // Error - no image created from pnm
      std::cout << "Error, not a proper pnm" << std::endl;
      return;
    }

    /*** Start from origional Apriltags from apriltag_demo.c ***/

    int hamm_hist[hamm_hist_max];
    memset(hamm_hist, 0, sizeof(hamm_hist));
    zarray_t *detections = apriltag_detector_detect(td, im);

    int chars_written;
    for (int i = 0; i < zarray_size(detections); i++) {

      apriltag_detection_t *det;
      zarray_get(detections, i, &det);
      sprintf(locationString, "Tag Center: (%f,%f)", centerPoint[0], centerPoint[1]);
      sprintf(detectString, "detection %2d: id (%2dx%2d)-%-4d, hamming %d, goodness %5.3f, margin %5.3f\n",
              i+1, det->family->d*det->family->d, det->family->h, det->id, det->hamming, det->goodness, det->decision_margin);

      hamm_hist[det->hamming]++;

      // draws a vertical rectangle around tag, not ideal, but easy to implement
      // det->p[corner][positon], counter clockwise
      pts.clear();
      for(int i = 0; i < 4; i++)
        pts.push_back(Point(det->p[i][0], det->p[i][1]));

      // If tag found flag, scale to image size for quick search
      if(found){
        Size s = src.size();
        int cpx = 5*opt1/3;
        int cpy = 5*opt2/3;
        pts[0] = Point(((det->p[0][0] + cpx)/w) * s.width, ((det->p[0][1] + cpy)/h) * s.height);
        pts[1] = Point(((det->p[1][0] + cpx)/w) * s.width, ((det->p[1][1] + cpy)/h) * s.height);
        pts[2] = Point(((det->p[2][0] + cpx)/w) * s.width, ((det->p[2][1] + cpy)/h) * s.height);
        pts[3] = Point(((det->p[3][0] + cpx)/w) * s.width, ((det->p[3][1] + cpy)/h) * s.height);
        centerPoint[0] = ((det->c[0] + cpx)/w) * s.width;
        centerPoint[1] = ((det->c[1] + cpy)/h) * s.height;
      }else{
        centerPoint[0] = det->c[0];
        centerPoint[1] = det->c[1];
      }

      if(init){
        initPts.push_back(pts[0]);
        initPts.push_back(pts[1]);
        initPts.push_back(pts[2]);
        initPts.push_back(pts[3]);
        init = false;
      }
      
      solvePnP(objPts, Mat(pts), cameraMatrix, distCoeffs, rvec, tvec, found);

      tvec.at<double>(0, 0) *= -1;
      Mat R;
      Rodrigues(rvec, R);
      R = R.t();
      tvec = -R * tvec;

      bool foundNaN = false;
      for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
          double d = rvec.at<double>(i, j);
          if (d != d) {
            foundNaN = true;
            rvec.at<double>(i, j) = 0.0;
          }
        }
      }

      // Write RT matrix to socket
      sprintf(buffer, "[%g,%g,%g,%g",
              rvec.at<double>(0,0),
              rvec.at<double>(1,0),
              rvec.at<double>(2,0),
	      tvec.at<double>(0));

      sprintf(buffer, "%s,%g,%g,%g,%g", buffer,
              rvec.at<double>(0,1),
              rvec.at<double>(1,1),
              rvec.at<double>(2,1),
              tvec.at<double>(1));

      chars_written = sprintf(buffer, "%s,%g,%g,%g,%g]\n", buffer,
              rvec.at<double>(0,2),
              rvec.at<double>(1,2),
              rvec.at<double>(2,2),
              tvec.at<double>(2));

      cv::rectangle(src, pts[0], pts[2], cvScalar(102,255,0));

      std::cout << "Rotation and Translation Matrix: " << std::endl;
      std::cout << "\t[ "<<rvec.at<double>(0,0)<<" "<<rvec.at<double>(1,0)<<" "<<rvec.at<double>(2,0)<<" ]";
      std::cout << "\t[ "<<tvec.at<double>(0,0)<<" ]"<<std::endl;
      std::cout << "\t[ "<< rvec.at<double>(0,1)<<" "<<rvec.at<double>(1,1)<<" "<<rvec.at<double>(2,1)<<" ]";
      std::cout << "\t[ "<<tvec.at<double>(0,1)<<" ]"<<std::endl;
      std::cout << "\t[ "<< rvec.at<double>(0,2)<<" "<<rvec.at<double>(1,2)<<" "<<rvec.at<double>(2,2)<<" ]";
      std::cout << "\t[ "<<tvec.at<double>(0,2)<<" ]"<<std::endl;

      apriltag_detection_destroy(det);
    }

    if(zarray_size(detections) < 1){
      found = false;
      centerPoint[0] = 0.0;
      centerPoint[1] = 0.0;
      sprintf(detectString, "No tag detected");
      sprintf(locationString, "No tag detected");
    }else{
      found = true;
      write(session_fd, buffer, chars_written);
   }

    zarray_destroy(detections);
    image_u8_destroy(im);

    t = clock() - t;
    time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);

    if (!quiet) {
      totalFPS += (1000.0/time_taken);
      count += 1.0;
      if(count > 150.0){
        totalFPS = 0.0;
        count = 0.0;
      }
      sprintf(displayString, "fps: %2.2Lf, nquads: %d",totalFPS/count, td->nquads);
    }

    sprintf(renderTime, "Render: %5.3fms", time_taken);
    sprintf(imgSize, "%dx%d", frame.cols, frame.rows);
    sprintf(outputString, "%s %s %s", renderTime, convertTime, imgSize);
    printf("%s %s\r", locationString, outputString);

    if (quiet) {
      printf("%12.3f", timeprofile_total_utime(td->tp) / 1.0E3);
    }

    printf("\n");

    /*** End of origional Apriltags from apriltag_demo.c ***/

    // displays fps, edges, segments, quads
    putText(src, displayString, cvPoint(30,30),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

    // displays render time, convert time, and image size
    putText(src, outputString, cvPoint(30,50),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

    // Displays any detections (if any)
    putText(src, detectString, cvPoint(30,70),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);


    // Displays tag location (if any)
    putText(src, locationString, cvPoint(30,90),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(150,150,250), 1, CV_AA);

    imshow("Display Apriltags", src);

    if(waitKey(30) >= 0) break;
  }

  /* deallocate apriltag constructs */
  apriltag_detector_destroy(td);
  tag36h11_destroy(tf);
}

int main(int argc, char * argv[]){

  const char* hostname = 0;
  const char* portname = MY_PORT;
  struct addrinfo hints;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
  struct addrinfo* res = 0;

  int err = getaddrinfo(hostname,portname,&hints,&res);

  if (err != 0) {
    printf("failed to resolve local socket address (err=%d)",err);
  }

  int server_fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (server_fd == -1) {
    printf("%s",strerror(errno));
  }

  int reuseaddr = 1;
  if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr))==-1) {
    printf("%s",strerror(errno));
  }

  if (bind(server_fd,res->ai_addr,res->ai_addrlen) == -1) {
    printf("%s",strerror(errno));
  }

  if (listen(server_fd,SOMAXCONN)) {
    printf("failed to listen for connections (errno=%d)",errno);
  }

  for (;;) {
    int session_fd = accept(server_fd,0,0);
    if (session_fd == -1) {
      if (errno == EINTR) continue;
      printf("failed to accept connection (errno=%d)",errno);
    }
    pid_t pid = fork();
    if (pid == -1) {
      printf("failed to create child process (errno=%d)",errno);
    } else if (pid == 0) {
      close(server_fd);
      handle_session(session_fd);
      close(session_fd);
      _exit(0);
    } else {
      close(session_fd);
    }
  }

  for (;;) {
    int session_fd = accept(server_fd,0,0);
    if (session_fd == -1) {
      if (errno == EINTR) continue;
      printf("failed to accept connection (errno=%d)",errno);
    }
    pid_t pid = fork();
    if (pid == -1) {
      printf("failed to create child process (errno=%d)",errno);
    } else if (pid == 0) {
      close(server_fd);
      handle_session(session_fd);
      close(session_fd);
      _exit(0);
    } else {
      close(session_fd);
    }
  }

  freeaddrinfo(res);

  return 0;
}
