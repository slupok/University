#include "ipImage.h"

IPImage::IPImage() : m_data(nullptr), m_width(0), m_height(0), m_pixelType(PixelType::Undefined)
{

}
IPImage::IPImage(uint width, uint height, PixelType type) : m_width(width), m_height(height), m_pixelType(type)
{
    m_data = (uchar*)malloc(height * width * IP_GetBytesPerPixel(type));
}
IPImage::~IPImage()
{
    if(m_data)
        free(m_data);
    m_data = nullptr;
}
uint IPImage::IP_GetBytesPerPixel(PixelType pixelType)
{
    if(pixelType == PixelType::Grayscale8)
        return 1;
    else if (pixelType == PixelType::Float32)
        return 4;
    else if (pixelType == PixelType::RGB24)
        return 3;
    else if (pixelType == PixelType::BGR24)
        return 3;
    else if (pixelType == PixelType::RGBA32)
        return 4;
    else
        return 0;
}
uint IPImage::GetWidth()
{
    return m_width;
}
uint IPImage::GetHeight()
{
    return m_height;
}
PixelType IPImage::GetPixelType()
{
    return m_pixelType;
}
