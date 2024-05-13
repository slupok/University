#ifndef RAYTRACE_H
#define RAYTRACE_H
#include <QImage>
#include <QPixmap>
#include <QMatrix4x4>
#include "other.h"


class RayTrace : public QObject
{
    Q_OBJECT
public:
    RayTrace();
    ~RayTrace();
    void setCameraModelMatrix(QMatrix4x4 m);
    void cameraRotate(float xAngle, float yAngle, float zAngle);
    void cameraMove(QVector3D move);

    QPixmap draw() const;
    void setImageSize(const int &width, const int &height);


private:
    int m_width;
    int m_height;

    float m_fov;
    QVector<Sphere> m_spheres;
    QVector<Light> m_light;

    Camera m_camera;
};

#endif // RAYTRACE_H
