#ifndef IP_IMAGE_H
#define IP_IMAGE_H
#include "imageProcessing.h"
#include <QImage>

class IPImage
{
protected:
    IPImage();
    IPImage(uint width, uint height, PixelType type);

public:
    virtual ~IPImage();
    static uint IP_GetBytesPerPixel(PixelType pixelType);//del
    uint GetWidth();
    uint GetHeight();
    PixelType GetPixelType();
    virtual IPEnum GetDeviceType() = 0;
    virtual IPError WriteImage(const void *data, size_t length) = 0;
    //доавть IsSizeEqual()

    //можно добавить функцию которая заполняем текущий QImage, чтобы каждый раз не создавать новый
    //соответсвенно нужны проверки, чтобы совпадали размеры и colorType
    virtual IPError ReadImage(void *data, size_t length) = 0;
   // virtual QImage* GetImage() = 0;
    //virtual IPError ReadMask(uchar* mask, int width, int height, PixelType pixelType) = 0;
protected:
    uchar *m_data;
    uint m_width, m_height;
    PixelType m_pixelType;
};

#endif
