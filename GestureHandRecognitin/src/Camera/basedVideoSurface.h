#ifndef BASEDVIDEOSURFACE_H
#define BASEDVIDEOSURFACE_H

#include <QDebug>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include <QLabel>
#include <QVideoSurfaceFormat>

#include "videoSurfaceCaptured.h"

class BasedVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    BasedVideoSurface(VideoSurfaceCaptured *surfaceCaptured);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats( QAbstractVideoBuffer::HandleType handleType) const;
    bool present(const QVideoFrame &frame);
private:
    VideoSurfaceCaptured *m_surfaceCaptured;
};

#endif // BASEDVIDEOSURFACE_H
