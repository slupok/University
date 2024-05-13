#ifndef IP_CONTEXT_H
#define IP_CONTEXT_H

#include "imageProcessing.h"
#include "ipDevice.h"
#include "ipImage.h"
//#include "../imageDataBuffer.h" //del

#include "OpenCL/opencl_device.h"

#include "math.h"


struct IPDevices{
    int openCL;
    int CUDA;
    int Metal;
};


class IPContext
{
public:
    IPContext();
    virtual ~IPContext();

    virtual void RunTest() = 0;//del

    virtual IPImage* CreateImage(uint w, uint h, PixelType pixelType, void *data) = 0;// create IPImage with used device type

    virtual IPEnum GetDeviceType() = 0;

    //image functions
    virtual IPError copyImage(IPImage *sourceImage, IPImage *destinationImage) = 0;


    //functions
 //   virtual IPError kMeans(int clustersCount, int iterationsCount, RGB_format recognitionColor) = 0;
    virtual IPError FrameDifference(IPImage *currentImage, IPImage *previousImage, IPImage *resultMask, uchar theshold) = 0;
    virtual IPError ColorThresholdConversion(IPImage *image, IPImage *resultMask) = 0;
    virtual IPError GaussianBlur(IPImage *image, int radius, float sigma) = 0;
    virtual IPError CenterOfBitmap(IPImage *mask, int &x, int &y) = 0;
    virtual IPError CentralMoment(IPImage *image, IPImage *mask, const int p, const int q, const int cx, const int cy, int &moment) = 0;

    //morphological functions
    virtual IPError MorphologicalDilation(IPImage *image, int radius) = 0;
    virtual IPError MorphologicalErosion(IPImage *image, int radius) = 0;

    //bitmap functions
    virtual IPError BitmapSubtraction(IPImage *bitmap, IPImage *subtrahendBitmap) = 0;
    virtual IPError BitmapIntersection(IPImage *bitmap, IPImage *tmpBitmap) = 0;

//protected:
  //  IPImage *m_image;
};

static const IPDevices SupportedDevices = {
    #ifdef USE_OPENCL
    1,
    #else
    0,
    #endif
    #ifdef USE_CUDA
    1,
    #else
    0,
    #endif
    #ifdef USE_METAL
    1
    #else
    0
    #endif
};

#endif
