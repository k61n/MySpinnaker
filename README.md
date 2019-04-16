# MySpinnaker
Class to acquire frames from FLIR BFS-U3-200S6C-C camera using Spinnaker SDK, Qt and OpenCV.

Class contains basic methods to acquire single image from camera, to set up exposition time and to reset back in autoexposure mode.
The images are returned as a cv::Mat object, which might be later shown on the screen or saved to disk.

The class should work with other FLIR cameras, however if used take care of PixelFormat of your camera and its conversion into your target fromat. See color conversion codes in cv::cvtColor method.

Installation paths for OpenCV and Spinnaker SDK files might vary in the pro-file.
