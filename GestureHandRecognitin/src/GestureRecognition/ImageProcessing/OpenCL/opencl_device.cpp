#include "opencl_device.h"

#include <QFile>
#include <QDebug>
OpenclDevice::OpenclDevice()
{
    IPError error = InitDevice();
    CHECK_ERROR(error);
}
OpenclDevice::~OpenclDevice()
{
    //release

    //kernel
    if(m_testKernel)
        clReleaseKernel(m_testKernel);
    m_testKernel = nullptr;

    //program
    if(m_imageProgram)
        clReleaseProgram(m_imageProgram);
    m_imageProgram = nullptr;

    //device
    if(m_device)
        clReleaseDevice(m_device);
    m_device = nullptr;

    //context
    if(m_context)
        clReleaseContext(m_context);
    m_context = nullptr;
}

IPEnum OpenclDevice::GetDeviceType()
{
    return IPDeviceOpenCL ;
}
IPError OpenclDevice::InitDevice()
{
    cl_int error;
    cl_uint num_platforms;
    error = clGetPlatformIDs(1, &m_platform, &num_platforms);
    if(error != 0)
        return CHECK_ERROR(IPErrorDevice);

    cl_uint num_devices;
    error = clGetDeviceIDs(m_platform, CL_DEVICE_TYPE_GPU, 1, &m_device, &num_devices);
    if(error != 0)
        return CHECK_ERROR(IPErrorDevice);

    m_context = clCreateContext(0x0, 1, &m_device, 0x0, 0x0, &error);
    if(error != 0)
        return CHECK_ERROR(IPErrorDevice);


    /* init program */

    QFile clFile;
    clFile.setFileName("gesture.cl");
    clFile.open(QIODevice::ReadOnly);
    QByteArray bytes = clFile.readAll();
    clFile.close();

    bool err = CreateProgram(bytes.data(), bytes.size());
    if(!err)
    {
        qDebug() << "Error create openCL program";
        return CHECK_ERROR(IPErrorDevice);
    }
    return IPNoError;
}

bool OpenclDevice::CreateProgram(char *source, size_t sourceSize)
{
    cl_int error;
    m_imageProgram = clCreateProgramWithSource(m_context, 1, (const char **)&source, (const size_t *)&sourceSize, &error);
    if(error != 0)
        return false;
    error = clBuildProgram(m_imageProgram, 1, &m_device, 0x0, 0x0, 0x0);
    if(error != 0)
        return false;

    m_testKernel = clCreateKernel(m_imageProgram, "test", &error);
    if(error != 0)
        return false;

    m_frameDifferenceKernel = clCreateKernel(m_imageProgram, "FrameDifferenceKernel", &error);
    if(error != 0)
        return false;

    m_thresholdColorKernel = clCreateKernel(m_imageProgram, "thresholdColorConversionKernel", &error);
    if(error != 0)
        return false;

    m_separableGaussianBlurKernel = clCreateKernel(m_imageProgram, "SeparableGaussianBlurKernel", &error);
    if(error != 0)
        return false;

    m_coordinateSummingKernel = clCreateKernel(m_imageProgram, "CoordinateSummingKernel", &error);
    if(error != 0)
        return false;

    m_bitmapCentralMomentKernel = clCreateKernel(m_imageProgram, "BitmapCentralMomentKernel", &error);
    if(error != 0)
        return false;

    //морфология
    m_morphologicalDilationKernel = clCreateKernel(m_imageProgram, "MorphologicalDilationKernel", &error);
    if(error != 0)
        return false;

    m_morphologicalErosionKernel = clCreateKernel(m_imageProgram, "MorphologicalErosionKernel", &error);
    if(error != 0)
        return false;

    //bitmap
    m_bitmapSubtractionKernel = clCreateKernel(m_imageProgram, "BitmapSubtractionKernel", &error);
    if(error != 0)
        return false;

    m_bitmapIntersectionKernel = clCreateKernel(m_imageProgram, "BitmapIntersectionKernel", &error);
    if(error != 0)
        return false;
    return true;
}
