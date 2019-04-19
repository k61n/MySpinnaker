#ifndef MYSPINNAKER_H
#define MYSPINNAKER_H

#include <QObject>
#include <QDebug>
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

class MySpinnaker : public QObject
{
    Q_OBJECT

public:
    MySpinnaker(QObject *parent = nullptr);
    ~MySpinnaker();
    bool streamingIsRequested = false;
    //int64_t pixelFormat; //PixelFormat_BayerGR8

    cv::Mat getImage();
    void stream();
    void setExposure(double exposureTimeToSet);
    void resetExposure();

signals:
    void on_capture(cv::Mat cvMat);

public slots:
    void on_start_streaming();
    void on_stop_streaming();

private:
    SystemPtr system;
    CameraPtr pCam;
    CameraList camList;
    INodeMap* nodeMapTLDevice;
    INodeMap* nodeMap;
};

#endif // MYSPINNAKER_H
