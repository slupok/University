#ifndef OPENCL_CONTEXT_H
#define OPENCL_CONTEXT_H

#include "../ipContext.h"
#include "opencl_device.h"
#include "opencl_image.h"


//хорошей идеей будет , чтобы функции принимали в качестве параметра IPImage,
//тогда их можно будет использовать несколько как параметры:
// ColorThresholdConersion(IPImage *image, IPImage *resultMask)

class OpenclContext : public IPContext
{
public:
    OpenclContext();
    ~OpenclContext() override;

    //static IPError CreateDevice();
    //static constexpr void ReleaseDevice();

    void RunTest() override;

    IPImage* CreateImage(uint32_t w, uint32_t h, PixelType pixelType, void *data) override;
    //IPError AttachImage(IPImage *image) override;

    IPEnum GetDeviceType() override;

    //image functions
    IPError copyImage(IPImage *sourceImage, IPImage *destinationImage) override;

    //functions
  //  IPError kMeans(int clustersCount, int iterationsCount, RGB_format recognitionColor) override;
    IPError FrameDifference(IPImage *currentImage, IPImage *previousImage, IPImage *resultMask, uchar theshold) override;
    IPError ColorThresholdConversion(IPImage *image, IPImage *resultMask) override;
    IPError GaussianBlur(IPImage *image, int radius, float sigma) override;
    IPError CenterOfBitmap(IPImage *mask, int &x, int &y) override;
    IPError CentralMoment(IPImage *image, IPImage *mask, const int p, const int q, const int cx, const int cy, int &moment) override;

    //morphological functions
    IPError MorphologicalDilation(IPImage *image, int radius) override;
    IPError MorphologicalErosion(IPImage *image, int radius) override;

    //bitmap functions
    IPError BitmapSubtraction(IPImage *bitmap, IPImage *subtrahendBitmap) override;
    IPError BitmapIntersection(IPImage *bitmap, IPImage *tmpBitmap) override;

private:
    //мб создать счетчик ссылок на device, и тогда освобжать его, когда больше нет ссылок?
    //static uint m_deviceReferenceCounted;

    //придумать как выделять устройство единожды
    OpenclDevice *m_device;

    cl_command_queue m_queue;

};


#endif //OPENCL_CONTEXT_H
