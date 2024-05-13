#include "basedVideoSurface.h"

BasedVideoSurface::BasedVideoSurface(VideoSurfaceCaptured *surfaceCaptured) :
    QAbstractVideoSurface()
{
    m_surfaceCaptured = surfaceCaptured;
}

QList<QVideoFrame::PixelFormat> BasedVideoSurface::supportedPixelFormats( QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555
                << QVideoFrame::Format_UYVY;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool BasedVideoSurface::present(const QVideoFrame &frame)
{

    QVideoFrame tmpFrame(frame);

    tmpFrame.map(QAbstractVideoBuffer::ReadOnly);

    QImage image(tmpFrame.bits(),
                 tmpFrame.width(),
                 tmpFrame.height(),
                 //frametodraw.bytesPerLine(),
                 QVideoFrame::imageFormatFromPixelFormat(tmpFrame.pixelFormat()));
    image = image.convertToFormat(QImage::Format_RGB888);
    if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop)
        image = image.transformed(QTransform().scale(-1,-1).translate(0, image.width()));
    else
        image = image.transformed(QTransform().scale(-1,1).translate(0, image.width()));
    tmpFrame.unmap();

    //мб лучше передавать сразу биты/размер/тип?
    m_surfaceCaptured->capture(image);
    return true;
}
