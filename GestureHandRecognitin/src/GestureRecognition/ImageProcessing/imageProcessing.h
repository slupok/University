#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#include "assert.h"
#include <QDebug>

#define USE_OPENCL

#define CHECK_ERROR(error) (check_error(error))


typedef enum IPError
{
    IPNoError = 0,
    IPErrorDevice = 1,
    IPErrorOutOfMemory = 2,
    IPErrorNotSupported = 3,
    IPErrorMemoryCopy = 4, // ошибка возникающая при неудачном копировании данных (несовпадает размер или тип)
    IPErrorExecute = 5

} IPError;

inline IPError check_error(IPError error)
{
    if(error != IPNoError)
    {
        qDebug() << error;
        assert(error == IPNoError);
    }
    return IPNoError;
}

typedef enum IPEnum
{
    IPUndefined = 0,

    IPDeviceOpenCL = 100

} IPEnum;
typedef enum PixelType
{
    Undefined,
    Grayscale8,
    Float32,
    RGB24,
    BGR24,
    RGBA32
} PixelType;

inline int GetBytesPerPixel(PixelType pixelType)
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
#endif
