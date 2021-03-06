#include "myspinnaker.h"

MySpinnaker::MySpinnaker(QObject *parent)
{
    Q_UNUSED(parent);

    // Retrieve singleton reference to system object
    system = System::GetInstance();
    // Retrieve list of cameras from the system
    camList = system->GetCameras();

    unsigned int numCameras = camList.GetSize();
    qDebug() << "Number of cameras detected: " << numCameras;

    // Finish if there are no cameras
    if(numCameras == 0){
        // Clear camera list before releasing system
        camList.Clear();

        // Release system
        system->ReleaseInstance();

        qDebug() << "No camera detected. Aborting the program";
    }

    pCam = nullptr;

    pCam = camList.GetByIndex(0);

    try{
        // Retrieve TL device nodemap and print device information
        nodeMapTLDevice = &pCam->GetTLDeviceNodeMap();
        // Initialize camera
        pCam->Init();
        // Retrieve GenICam nodemap
        nodeMap = &pCam->GetNodeMap();

        //pixelFormat = pCam->PixelFormat.GetValue();
        qDebug() <<"Initialized Spinnaker compatible camera ("
                 << pCam->Width.GetValue() << "x" << pCam->Height.GetValue()
                 << pCam->PixelFormat.GetCurrentEntry()->GetSymbolic()
                 << ")";
        qDebug() <<"Exposure time" << pCam->ExposureTime.GetValue() << "us";
    }
    catch (Spinnaker::Exception &e){
        qDebug() << "Error: " << e.what() ;
    }
}

MySpinnaker::~MySpinnaker()
{
    pCam->DeInit();
    pCam = nullptr;
    nodeMapTLDevice = nullptr;
    nodeMap = nullptr;
    camList.Clear();
    system->ReleaseInstance();
}

cv::Mat MySpinnaker::getImage()
{
    //Acquisition begins if camera is not streaming
    if (!pCam->IsStreaming())
        pCam->BeginAcquisition();
    //spinnaker class for acquired image
    ImagePtr img = pCam->GetNextImage();
    //opencv class for acuired image
    cv::Mat cvMat;

    if (img->IsIncomplete()){
        qDebug() << "Image incomplete with image status " << img->GetImageStatus();
    }
    else{
        int cvFormat = CV_8UC1; //openCV format that corresponds to BayerRG8 for Backfly S BFS-U3-200S6C-C
        unsigned int XPadding = img->GetXPadding();
        unsigned int YPadding = img->GetYPadding();
        unsigned int rowsize = img->GetWidth();
        unsigned int colsize = img->GetHeight();
        //conversion from ImagePtr to cv::Mat
        cvMat = cv::Mat(colsize + YPadding, rowsize + XPadding, cvFormat, img->GetData(), img->GetStride());
        //conversion of channels from BayerRG8 to RGB
        cv::cvtColor(cvMat, cvMat, cv::COLOR_BayerRG2RGB_EA);
    }
    //Release spinnaker image class
    img->Release();
    //Acquisition ends if streaming is not required
    if (!streamingIsRequested)
        pCam->EndAcquisition();

    return cvMat;
}

//method for capturing images in a separate thread
void MySpinnaker::stream()
{
    while (streamingIsRequested)
        emit on_capture(getImage());
}

void MySpinnaker::on_start_streaming()
{
    streamingIsRequested = true;
}

void MySpinnaker::on_stop_streaming()
{
    streamingIsRequested = false;
}

void MySpinnaker::setExposure(double exposureTimeToSet)
{
    //Exposure configuration
    try{
        //Disabling autoexposure
        CEnumerationPtr ptrExposureAuto = nodeMap->GetNode("ExposureAuto");
        if(!IsAvailable(ptrExposureAuto) || !IsWritable(ptrExposureAuto))
            qDebug() << "Unable to disable automatic exposure (node retrieval). Aborting...";
        CEnumEntryPtr ptrExposureAutoOff = ptrExposureAuto->GetEntryByName("Off");
        if(!IsAvailable(ptrExposureAutoOff) || !IsReadable(ptrExposureAutoOff))
            qDebug() << "Unable to disable automatic exposure (enum entry retrieval). Aborting...";
        ptrExposureAuto->SetIntValue(ptrExposureAutoOff->GetValue());
        qDebug() << "Automatic exposure disabled...";

        //Setting the exposure time
        CFloatPtr ptrExposureTime = nodeMap->GetNode("ExposureTime");
        if(!IsAvailable(ptrExposureTime) || !IsWritable(ptrExposureTime))
            qDebug() << "Unable to set exposure time. Aborting...";
        // Ensure desired exposure time does not exceed the maximum
        const double exposureTimeMax = ptrExposureTime->GetMax();
        if(exposureTimeToSet > exposureTimeMax){
            exposureTimeToSet = exposureTimeMax;
        }
        ptrExposureTime->SetValue(exposureTimeToSet);
        qDebug() << "Exposure time set to " << exposureTimeToSet << " us...";
    }
    catch(Spinnaker::Exception &e){
        qDebug() << "Error: " << e.what();
    }
}

void MySpinnaker::resetExposure()
{
    try{
        // Turn automatic exposure back on
        CEnumerationPtr ptrExposureAuto = nodeMap->GetNode("ExposureAuto");
        if (!IsAvailable(ptrExposureAuto) || !IsWritable(ptrExposureAuto))
            qDebug() << "Unable to enable automatic exposure (node retrieval). Non-fatal error...";
        CEnumEntryPtr ptrExposureAutoContinuous = ptrExposureAuto->GetEntryByName("Continuous");
        if (!IsAvailable(ptrExposureAutoContinuous) || !IsReadable(ptrExposureAutoContinuous))
            qDebug() << "Unable to enable automatic exposure (enum entry retrieval). Non-fatal error...";

        ptrExposureAuto->SetIntValue(ptrExposureAutoContinuous->GetValue());
        qDebug() << "Automatic exposure enabled...";
    }
    catch (Spinnaker::Exception &e){
            qDebug() << "Error: " << e.what();
    }
}
