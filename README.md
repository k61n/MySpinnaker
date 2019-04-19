# MySpinnaker
Class to acquire frames from FLIR BFS-U3-200S6C-C camera using Spinnaker SDK, Qt and OpenCV.

Class contains methods to acquire single image from camera, to stream images, to set up exposition time and to reset back in autoexposure mode.
In this example the images are returned as a cv::Mat object, which then are displayed in QGraphicsView. Streaming runs in a separate thread.

The class should work with other FLIR cameras, however if used take care of PixelFormat of your camera and its conversion into your target fromat. See color conversion codes in cv::cvtColor method.

Installation paths for OpenCV and Spinnaker SDK files may be different from the paths in my pro-file.
