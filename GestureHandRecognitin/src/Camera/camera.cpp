#include "camera.h"

Camera::Camera()
{
    m_surface = new BasedVideoSurface(this);
    //m_callback = new CameraCallback();
}

void Camera::capture(QImage image)
{
    //ImageBuffer frame;
    //newImageBuffer(&frame,image.bits(),image.width(),image.height(),PixelTypeRGB);
    emit callbackFrame(image);
    //releaseImageBuffer(&frame);
    //delete &image;
}


void Camera::start()
{
    m_camera->start();
}

void Camera::stop()
{
    m_camera->stop();
}

void Camera::setCamera(QCameraInfo cameraInfo)
{
    m_camera = new QCamera(cameraInfo);
    m_camera->setViewfinder(m_surface);
}

Camera::~Camera()
{
    delete m_surface;
    delete m_camera;
}
