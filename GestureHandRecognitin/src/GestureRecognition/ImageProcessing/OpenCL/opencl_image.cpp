#include "opencl_image.h"

OpenclImage::OpenclImage() : IPImage()
{
    m_image = nullptr;
    m_queue = nullptr;
}
OpenclImage::OpenclImage(cl_command_queue queue, cl_mem img, uint width, uint height, PixelType type)
    : IPImage(width, height, type)
{
    m_queue = queue;
    m_image = img;
}
OpenclImage::~OpenclImage()
{
    if(m_image)
        clReleaseMemObject(m_image);
    m_image = nullptr;

//    if(m_backgroundImage)
//        clReleaseMemObject(m_backgroundImage);
//    m_backgroundImage = nullptr;

    //очерь от контекста, пожто ее лучше освобождать в контексте
#if 0
    if(m_queue)
        clReleaseCommandQueue(m_queue);
    m_queue = nullptr;
#endif
    m_queue = nullptr;
}
IPEnum OpenclImage::GetDeviceType()
{
    return IPDeviceOpenCL;
}


IPError OpenclImage::WriteImage(const void *data, size_t length)
{
    cl_int error;
    if(length != m_width * m_height * IP_GetBytesPerPixel(m_pixelType))
        return CHECK_ERROR(IPErrorMemoryCopy);

    error = clEnqueueWriteBuffer(m_queue, m_image, CL_TRUE, 0, length, data, 0, nullptr, nullptr);
    if(error != 0)
       return CHECK_ERROR(IPErrorOutOfMemory);

    return IPNoError;

}
#if 0
IPError OpenclImage::GetData(void *data)
{
    if(!data)
        return CHECK_ERROR(IPErrorOutOfMemory);

    cl_int error = clEnqueueReadBuffer(m_queue, m_image, CL_TRUE, 0,
                                       m_width * m_height * GetBytesPerPixel(m_pixelType), data, 0, nullptr, nullptr);
    if(error != 0)
        return CHECK_ERROR(IPErrorOutOfMemory);

    return IPNoError;
}
#endif
IPError OpenclImage::ReadImage(void *data, size_t length)
{
    if(!data || m_width * m_height * IP_GetBytesPerPixel(m_pixelType) != length)
        return CHECK_ERROR(IPErrorOutOfMemory);

    cl_int error = clEnqueueReadBuffer(m_queue, m_image, CL_TRUE, 0,
                                       length, data, 0, nullptr, nullptr);
    if(error != 0)
        return CHECK_ERROR(IPErrorOutOfMemory);

    return IPNoError;
}

//delete
#if 0
QImage* OpenclImage::GetImage()
{
    IPError error = IPNoError;
    error = GetData(m_data);

    QImage::Format format;
    QImage *img;

    if(m_pixelType == PixelType::RGB24)
    {
        format = QImage::Format_RGB888;
        //need inverted format

        img = new QImage(m_data, m_width, m_height, format);
        //img->invertPixels(QImage::InvertRgb);
    }
    else if(m_pixelType == PixelType::BGR24)
    {
        format = QImage::Format_BGR888;
        img = new QImage(m_data, m_width, m_height, format);
    }
    else if(m_pixelType == PixelType::RGBA32)
    {
        format = QImage::Format_RGBA8888;
        img = new QImage(m_data, m_width, m_height, format);
    }
    else if(m_pixelType == PixelType::Grayscale8)
    {
        format = QImage::Format_Grayscale8;
        img = new QImage(m_data, m_width, m_height, format);
    }
    else
        return nullptr;

    return img;

}
#endif
