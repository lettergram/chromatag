/**
 * Written by Austin Walters
 * For use in RMS Lab @ UIUC
 *
 * To Execute: g++ -I/usr/include -L/usr/lib -lopencv_core.2.4.9 -lopencv_calib3d.2.4.9 -lopencv_imgproc.2.4.9 -lopencv_highgui.2.4.9 rgb2lab.cpp -o rgb2lab
 */

#include <iostream>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define CV_LOAD_IMAGE_COLOR 1

/* version 2.4 of openCV */
using namespace cv;

/**
 * Converts a single pixel from RGB to Lab
 */
Point3_<uchar> RGB2Lab(Point3_<uchar> pixel){

  float r = pixel.x;
  float g = pixel.y;
  float b = pixel.z;

  float varR = (r / 255.0);
  float varG = (g / 255.0);
  float varB = (b / 255.0);

  // RGB to XYZ
  if (varR > 0.04045){varR = pow(((varR + 0.055)/1.055), 2.4);}
  else{ varR = (varR / 12.92); }
  if (varG > 0.04045){varG = pow(((varG + 0.055)/1.055), 2.4);}
  else{ varG = (varG / 12.92); }
  if (varB > 0.04045){varB = pow(((varB + 0.055)/1.055), 2.4);}
  else{ varB = (varB / 12.92); }


  varR *= 100.0;
  varG *= 100.0;
  varB *= 100.0;


  float X = (varR*0.4124 + varG*0.3576 + varB*0.1805);
  float Y = (varR*0.2126 + varG*0.7152 + varB*0.0722);
  float Z = (varR*0.0193 + varG*0.1192 + varB*0.9505);

  uchar L = (uchar)((int)(10.0 * sqrt(Y)));
  uchar A = (uchar)((int)(17.5 * (((1.02*X) - Y) / sqrt(Y))));
  uchar B = (uchar)((int)(7.0 * ((Y - (0.847*Z)) / sqrt(Y))));

  Point3_<uchar> Lab;

  Lab.x = L;
  Lab.y = A;
  Lab.z = B;

  return Lab;
}

/**
 * My alternative method for converting to lab
 **/
Mat RGB2LAB_alt(Mat const &imgRGB){
  Size s = imgRGB.size();
  Mat imgLab(s, CV_8UC3);

  for(int y = 0; y < s.height; y++){
    for(int x = 0; x < s.width; x++){
      Point3_<uchar> pixel = RGB2Lab((*imgRGB.ptr<Point3_<uchar> >(y,x)));
      (*imgLab.ptr<Point3_<uchar> >(y,x)).x = pixel.x;
      (*imgLab.ptr<Point3_<uchar> >(y,x)).y = pixel.y;
      (*imgLab.ptr<Point3_<uchar> >(y,x)).z = pixel.z;
    }
  }

  return imgLab;
}

/**
 * Takes in a RGB image and outputs
 * a Lab space image.
 **/
Mat RGB2YUV(Mat const &imgRGB){
  Mat imgYUV;
  cvtColor(imgRGB, imgYUV, CV_BGR2YCrCb);
  return imgYUV;
}

/**                                                                                                 
 * Takes in a RGB image and outputs                                                                 
 * a Lab space image.                                                                               
 **/
Mat RGB2LAB(Mat const &imgRGB){
  Mat imgLab;
  cvtColor(imgRGB, imgLab, CV_BGR2Lab);
  return imgLab;
}

/**
 * Finds the gradient via the sobel method,
 * the output is a Mat with the gradient edges (might be better as array).
 * It must be gray!
 **/
Mat gradientEdges(Mat &img){
  
  Mat gradImg;
  Mat gx, gy;
  Mat abs_gx, abs_gy;
  
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  
  GaussianBlur(img, img, Size(3,3), 0, 0, BORDER_DEFAULT);
  
  // gradient in the x direction
  Sobel(img, gx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs( gx, abs_gx );
  
  // Gradient in the y direction
  Sobel( img, gy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs( gy, abs_gy );
  
  /// Total Gradient (approximate)
  addWeighted( abs_gx, 0.5, abs_gy, 0.5, 0, gradImg);
  
  return gradImg;
  
}

void unsharpMask(cv::Mat& im){
  cv::Mat tmp;
  cv::GaussianBlur(im, tmp, cv::Size(5,5), 5);
  cv::addWeighted(im, 1.5, tmp, -0.5, 0, im);
}

/**
 * Takes in image in the lab space and splits 
 * out the alpha stream. Returning it as a matrix,
 * to be saved out to file or manipulated
 **/
Mat alphaLAB(Mat const &imgLab){

  Mat alphaLab(imgLab);
  
  vector<Mat> channels(3);
  split(imgLab, channels);
  
  cvtColor(channels[1], alphaLab, CV_GRAY2RGB);
  
  unsharpMask(alphaLab);
  
  return alphaLab;
}

/**
 * Takes in image in the lab space and splits 
 * out the beta stream. Returning it as a matrix,
 * to be saved out to file or manipulated
 **/
Mat betaLAB(Mat &imgLab){
  
  Mat betaLab(imgLab);
  
  vector<Mat> channels(3);
  split(imgLab, channels);
  
  cvtColor(channels[2], betaLab, CV_GRAY2RGB);
  
  unsharpMask(betaLab);

  return betaLab;
}

/**
 * Takes in a RGB image and outputs
 * a Lab space image, only in the a channel
 **/
Mat RGB2LAB_a(Mat const &imgRGB){
  Mat imgLab;
  cvtColor(imgRGB, imgLab, CV_BGR2Lab);
  return alphaLAB(imgLab);
}

/** 
 * testLab Video - Tests video capture
 *  
 * @Input showGradient 
 *     If true, shows gradient of alpha channel
 *     If false, shows alpha channel
 **/
void testLabVideo(bool showGradient = false){
  
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if camera opened
    return;
  
  Mat a, b, g, frame;
  
  while(1){
    
    clock_t t;
    t = clock();
    
    cap >> frame; // get a new frame from camera
    Mat imgLab =  RGB2LAB(frame);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "time to convert: " << time_taken << std::endl;
    
    a = alphaLAB(imgLab);
    b = betaLAB(imgLab);
    
    t = clock();
    
    if(showGradient){
      g = gradientEdges(a);
    }else{
      g = a;
    }
    
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "time to find gradient: " << time_taken << std::endl;
    
    imshow("Display Edges", g);
    
    if(waitKey(30) >= 0) break;
    
  }
}
