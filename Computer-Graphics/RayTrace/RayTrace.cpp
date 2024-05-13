#include "RayTrace.h"
#include "qpixmap.h"
#include <QtMath>
#include "other.h"
#include <QDebug>

Material ivory(QVector3D(0.6f, 0.3f, 0.1f), QVector3D(0.4f, 0.4f, 0.3f), 50.0f);
Material mirror(QVector3D(0.0f, 10.0f, 0.7f), QVector3D(1.f, 1.f, 1.0f), 425.0f);
Material saphire(QVector3D(0.6f, 0.3f, 0.1f), QVector3D(0.2f, 0.2f, 0.5f), 50.0f);

RayTrace::RayTrace()
{
    m_width = 1280;
    m_height = 720;
    m_fov = M_PI/2;

    m_camera.position   = QVector3D(0.0f, 0.0f, 0.0f);
    m_camera.forwardDir = QVector3D(0.0f, 0.0f, -1.0f);
    m_camera.UpDir      = QVector3D(0.0f, 1.0f, 0.0f);
    m_camera.rightDir   = QVector3D(1.0f, 0.0f, 0.0f);
    //m_camera.setModelMatrix(m_cameraTranslateMatrix);

    m_spheres.append(Sphere(QVector3D(-3,    0,   -6), 2, ivory));
    m_spheres.append(Sphere(QVector3D(-1.0, -1.5, -2), 2, mirror));//
    m_spheres.append(Sphere(QVector3D( 1.5, -0.5, -8), 3, saphire ));
    m_spheres.append(Sphere(QVector3D( 7,    5,   -2), 4, mirror));//


    m_light.append(Light(QVector3D(-20, 20, 20), 1.5f));
    //m_light.append(new Light(QVector3D(30, 50, -25), 1.8f));
    //m_light.append(new Light(QVector3D( 30, 20,  30), 1.7f));


}
RayTrace::~RayTrace()
{

}

inline bool rayIntersectSphere(const QVector3D origin, const QVector3D direction, float &t0,const Sphere &sphere)
{
#if 0
    QVector3D L = sphere.center - origin;
    float tc = QVector3D::dotProduct(L, direction);
    if(tc < 0) return false;

    float d2 = QVector3D::dotProduct(L,L) - tc*tc;
    if(d2 > sphere.radius*sphere.radius) return false;
    float t1c = qSqrt(sphere.radius*sphere.radius - d2);
    t0 = tc - t1c;
    float t1 = tc + t1c;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    return true;
#else
    QVector3D center = sphere.position();
    float radius = sphere.radius();

    QVector3D L = center - origin;
    float tca = QVector3D::dotProduct(L,direction);
    float d2 = QVector3D::dotProduct(L,L) - tca*tca;
    if (d2 > radius*radius) return false;
    float thc = sqrtf(radius*radius - d2);
    t0       = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    return true;
#endif

}

QVector3D reflect(const QVector3D &vector, const QVector3D &normal) {
    return vector - normal * 2.f * QVector3D::dotProduct(vector, normal);
}

inline bool ray_intersect(const QVector3D &origin, const QVector3D &direction, const QVector<Sphere> &spheres, QVector3D &hitPoint, QVector3D &normal, Material &material)
{
    float depth = std::numeric_limits<float>::max();
    //QColor color = background.color;
    int i;
    for(i = 0; i < spheres.count(); i++)
    {
        Sphere sphere = spheres.at(i);
        float iDepth;
        if (rayIntersectSphere(origin, direction, iDepth, sphere) && iDepth < depth)
        {
            depth = iDepth;
            hitPoint = origin + direction*iDepth;
            normal = (hitPoint - sphere.position()).normalized();
            material = sphere.material();

        }
    }

    return depth < 1000;
}

inline QVector3D cast_ray(const QVector3D &origin, const QVector3D &direction, const QVector<Sphere> &spheres, const QVector<Light> &lights, int rayIteration, const int &maxIteraionCount)
{
    QVector3D point, N;
    Material material;

    if(rayIteration > maxIteraionCount || !ray_intersect(origin, direction, spheres, point, N, material))
    {
        return QVector3D(0.2f, 0.7f, 0.8f);
    }


    QVector3D reflectDirection = reflect(direction, N).normalized();
    QVector3D reflectOrigin = QVector3D::dotProduct(reflectDirection, N) < 0 ? point - N * 1e-3 : point + N * 1e-3; // offset the original point to avoid occlusion by the object itself
    QVector3D reflectColor = cast_ray(reflectOrigin, reflectDirection, spheres, lights, rayIteration + 1, maxIteraionCount);

    int i = 0;
    float diffuse_intensity = 0;
    float specular_intensity = 0;
    for(i = 0; i < lights.count(); i++)
    {
        /* Light */
        Light light = lights.at(i);
        QVector3D lightDir = (light.position() - point).normalized();

        /* Shadow */
        float lightDistance = (light.position() - point).length();
        //Т.к. точка лежит на поверхности геометрии, и для того чтобы не допустить численные погрешности
        //необходимо небольшое смещение точки по нормали.
        QVector3D shadowOrigin = QVector3D::dotProduct(lightDir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
        QVector3D shadowPoint, shadowNormal;
        Material shadowMaterial;
        if (ray_intersect(shadowOrigin, lightDir, spheres, shadowPoint, shadowNormal, shadowMaterial)
                && (shadowPoint-shadowOrigin).length() < lightDistance)
            continue;

        /* Phong Shading */
        diffuse_intensity  += light.intensity() * std::max(0.f, QVector3D::dotProduct(lightDir, N));
        specular_intensity += powf(std::max(0.f, QVector3D::dotProduct(-reflect(-lightDir, N), direction)), material.specular) * light.intensity();
    }

    QVector3D color = (material.diffuse * diffuse_intensity * material.albedo.x()) +
            (QVector3D(1.0f, 1.0f, 1.0f) * specular_intensity * material.albedo.y())
            + reflectColor * material.albedo.z();

    return color;
}

QPixmap RayTrace::draw() const
{
    QImage img(m_width, m_height, QImage::Format_RGB888);
    uchar *data = img.bits();
    int width = m_width;
    int height = m_height;
    float fov = m_fov;
    QVector3D cameraPos =  m_camera.pos();

    QVector<Sphere> spheres;
    QVector<Light> lights;
    for(int i = 0; i < m_spheres.count(); i++)
    {
        Sphere sphere = m_spheres.at(i);
        sphere.setPosition( sphere.position());
        spheres.append(sphere);
        qDebug() << "position " << i << " = " << sphere.position();
    }
    for(int i = 0; i < m_light.count(); i++)
    {
        Light light = m_light.at(i);
        light.setPosition( light.position());
        lights.append(light);
    }
    qDebug() << "camera " <<  " = " << cameraPos;


    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
        {
            int index = j * width + i;

            float x =  (2*(i + 0.5f)/(float)width  - 1)*tan(fov/2.0f)*width/(float)height;
            float y = -(2*(j + 0.5f)/(float)height - 1)*tan(fov/2.0f);

            QVector3D direction = (m_camera.getRotateMatrix() * QVector3D(x, y, -1) ).normalized();
            QVector3D pixelColor = cast_ray(cameraPos, direction, spheres, lights, 0, 1);

            float max = std::max(pixelColor.x(), std::max(pixelColor.y(), pixelColor.z()));
            if (max > 1) pixelColor = pixelColor * (1.0f / max);
            int red = std::clamp((int)(pixelColor.x() * 0xff), 0, 255);
            int green = std::clamp((int)(pixelColor.y() * 0xff), 0, 255);
            int blue = std::clamp((int)(pixelColor.z() * 0xff), 0, 255);

            data[index * 3 + 0] = (int)red;
            data[index * 3 + 1] = (int)green;
            data[index * 3 + 2] = (int)blue;

        }

    QPixmap pixmap = QPixmap::fromImage(img);
    return pixmap;
}

void RayTrace::setImageSize(const int &width, const int &height)
{
    m_width = width;
    m_height = height;
}

void RayTrace::setCameraModelMatrix(QMatrix4x4 m)
{
    m_camera.applyModelMatrix(m);
}

void RayTrace::cameraRotate(float xAngle, float yAngle, float zAngle)
{
    float c = 0.006f;
    QVector3D pos = m_camera.pos();
#if 0
    QMatrix4x4 translateMatrix(1, 0, 0, -pos.x()*0.5,
                               0, 1, 0, -pos.y()*0.5,
                               0, 0, 1, -pos.z()*0.5,
                               0, 0, 0, 1);
#endif

    QMatrix4x4 yRotateMatrix(qCos(-yAngle*c), 0, -qSin(-yAngle*c), 0,
                                           0, 1,                0, 0,
                             qSin(-yAngle*c), 0,  qCos(-yAngle*c), 0,
                                           0, 0,                0, 1);

    QMatrix4x4 xRotateMatrix(1,                0,                 0, 0,
                             0, qCos(xAngle*c), -qSin(xAngle*c), 0,
                             0, qSin(xAngle*c),  qCos(xAngle*c), 0,
                             0,                0,                 0, 1);

    //QMatrix4x4 resultMatrix = translateMatrix.inverted() * yRotateMatrix * xRotateMatrix * translateMatrix;
    m_camera.applyRotateMatrix( (yRotateMatrix * xRotateMatrix).transposed());
}
void RayTrace::cameraMove(QVector3D move)
{
    QVector3D forward = m_camera.rotateMatrix * m_camera.forward();
    QVector3D right = m_camera.rotateMatrix * m_camera.right();
    QVector3D up = m_camera.rotateMatrix * m_camera.up();
    right *= move.x();
    up *= move.y();
    forward *= move.z();

    QMatrix4x4 tm0(1, 0, 0, -right.x()*0.5,
                   0, 1, 0, -right.y()*0.5,
                   0, 0, 1, -right.z()*0.5,
                   0, 0, 0, 1);
    QMatrix4x4 tm1(1, 0, 0, -up.x()*0.5,
                   0, 1, 0, -up.y()*0.5,
                   0, 0, 1, -up.z()*0.5,
                   0, 0, 0, 1);
    QMatrix4x4 tm2(1, 0, 0, forward.x()*0.5,
                   0, 1, 0, forward.y()*0.5,
                   0, 0, 1, forward.z()*0.5,
                   0, 0, 0, 1);

    m_camera.applyModelMatrix((tm0 * tm1 * tm2).transposed());
}
