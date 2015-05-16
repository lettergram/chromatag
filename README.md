ChromaTags
====

ChromaTags are a fiducial marker placed in the scene by a user, enabling a computer vision (or really any system) to use it as a point of reference. ChromaTags are derived from Apriltags, and are designed to be used to identify and identify position of robots or specific locations. Each tag is encoded with information, and this encoded information can be used to not only identify the robot, but the tag itself can also be used to identify ones relative position. 

It's possible to determine the position based a method called [homography](http://en.wikipedia.org/wiki/Homography_%28computer_vision%29), or pose estimation. 

Our implementation takes most of the code from the origional [Apriltags website](http://april.eecs.umich.edu/wiki/index.php/AprilTags), however we make it run in real time. ChromaTags takes advantage of colors, as opposed to simply black and white images, explained more here.

### To Build:  
1. `chromatags/comparison $ make`  
2. `mkdir build && cd build`  
3. `cmake .. && make`  

####Invoke Single Image: AprilTags

$ ./apriltags_demo [options] input_image.pnm

Options: 
	 -d: Debug, displays image at every step

####Invoke Single Image: ChromaTags
$ ./chromatags_demo [options] input_image.pnm

Options: 
	 -d: Debug, displays image at every step

####Invoke Realtime Tracking

$ cd comparison/build/

$ ./chromatag

or

$ ./apriltag

Boxes should apear around the tags, along with fps, score, etc.
