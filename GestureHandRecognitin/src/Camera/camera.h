 #ifndef CAMERA_H
#define CAMERA_H

#include <QCamera>
#include <QCameraInfo>

#include "videoSurfaceCaptured.h"
#include "basedVideoSurface.h"
//#include "../GestureRecognition/imageDataBuffer.h"

class GestureRecognition;

class CameraHandler : public QObject
{
    Q_OBJECT
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void setCamera(QCameraInfo cameraInfo) = 0;
signals:
    void callbackFrame(QImage frame);

};

class Camera : VideoSurfaceCaptured, public CameraHandler
{
public:
    Camera();
    ~Camera();

    void capture(QImage) override;
    void start() override;
    void stop() override ;
    void setCamera(QCameraInfo cameraInfo) override;

private:
    QCamera *m_camera;
    BasedVideoSurface *m_surface;


};

#endif // CAMERA_H
