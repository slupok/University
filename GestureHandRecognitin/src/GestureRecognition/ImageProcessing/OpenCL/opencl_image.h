#ifndef OPENCL_IMAGE_H
#define OPENCL_IMAGE_H
#include <CL/cl.h>
#include "../imageProcessing.h"
#include "../ipImage.h"

class OpenclContext;

class OpenclImage : public IPImage
{
private:
    OpenclImage();
    OpenclImage(cl_command_queue queue, cl_mem img, uint width, uint height, PixelType type);
public:
    ~OpenclImage() override;

    IPEnum GetDeviceType() override;

    IPError WriteImage(const void *data, size_t length) override;
    IPError ReadImage(void *data, size_t length) override;
    //QImage* GetImage() override;

private:
    //IPError GetData(void *data);

    cl_mem m_image;
    //cl_mem m_backgroundImage;
    cl_command_queue m_queue;//этот queue переходит от context

    friend OpenclContext;
};

#endif
