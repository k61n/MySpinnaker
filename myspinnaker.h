#ifndef MYSPINNAKER_H
#define MYSPINNAKER_H

#include <QObject>
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

class MySpinnaker : public QObject
{
public:
    MySpinnaker(QObject *parent = nullptr);
    ~MySpinnaker();
    bool isInitialized = false;
    unsigned int width = 0;
    unsigned int height = 0;
    float exposureTime = 0;
    int64_t pixelFormat; //PixelFormat_BGR8

    cv::Mat getImage();
    void setExposure(double exposureTimeToSet);
    void resetExposure();

private:
    SystemPtr system;
    CameraPtr pCam;
    CameraList camList;
    INodeMap* nodeMapTLDevice;
    INodeMap* nodeMap;

};

#endif // MYSPINNAKER_H
