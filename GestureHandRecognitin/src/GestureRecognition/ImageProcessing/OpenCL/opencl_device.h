#ifndef OPENCL_DEVICE_H
#define OPENCL_DEVICE_H

#include <CL/cl.h>

#include "../imageProcessing.h"
#include "../ipDevice.h"

class OpenclDevice
{
public:
    OpenclDevice();
    ~OpenclDevice();

    IPError InitDevice();
    IPEnum GetDeviceType();
public:
    bool CreateProgram(char *source, size_t sourceSize);

    cl_platform_id m_platform;
    cl_device_id m_device;
    cl_context m_context;
    //queue
    //int device;
    cl_program m_imageProgram;
    cl_kernel m_testKernel;//del
    cl_kernel m_kMeansKernel;//del
    cl_kernel m_frameDifferenceKernel;
    cl_kernel m_thresholdColorKernel;
    cl_kernel m_separableGaussianBlurKernel;
    cl_kernel m_coordinateSummingKernel;
    cl_kernel m_bitmapCentralMomentKernel;

    cl_kernel m_morphologicalErosionKernel;
    cl_kernel m_morphologicalDilationKernel;

    cl_kernel m_bitmapSubtractionKernel;
    cl_kernel m_bitmapIntersectionKernel;
};

#endif //GESTURE_CL_H
