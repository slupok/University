#ifndef VIDEOSURFACECAPTURED_H
#define VIDEOSURFACECAPTURED_H

#include <QImage>

class VideoSurfaceCaptured
{
public:
    virtual void capture(QImage) = 0;
};

#endif // VIDEOSURFACECAPTURED_H
