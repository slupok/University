#ifndef IP_DEVICE_H
#define IP_DEVICE_H
#include "imageProcessing.h"

class IPDevice
{
public:
    IPDevice();
    ~IPDevice();
protected:
    virtual IPError InitDevice() = 0;
    virtual IPEnum GetDeviceType() = 0;

};

#endif
