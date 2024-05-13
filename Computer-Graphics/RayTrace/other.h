#ifndef OTHER_H
#define OTHER_H
#include <QVector3D>
#include <QVector2D>
#include <QColor>
#include <QMatrix4x4>

struct Camera {

    QVector3D position;
    QVector3D forwardDir;
    QVector3D rightDir;
    QVector3D UpDir;

    QMatrix4x4 modelMatrix;
    QMatrix4x4 rotateMatrix;

    Camera() {
        modelMatrix.setToIdentity();
        rotateMatrix.setToIdentity();
        position = QVector3D(0,0,0);
        forwardDir = QVector3D(0,0,-1);
        rightDir = QVector3D(1,0,0);
        UpDir = QVector3D(0,1,0);
    }

    void applyModelMatrix(QMatrix4x4 mm)
    {
        modelMatrix = modelMatrix * mm;
    }

    void applyRotateMatrix(QMatrix4x4 mm)
    {
        rotateMatrix = rotateMatrix * mm;
    }

    QMatrix4x4 getModelMatrix() const
    {
        return modelMatrix;
    }

    QMatrix4x4 getRotateMatrix() const
    {
        return rotateMatrix;
    }

    QVector3D pos() const
    {
        return position * modelMatrix;
    }

    QVector3D forward() const
    {
        return forwardDir;
    }

    QVector3D up() const
    {
        return UpDir;
    }

    QVector3D right() const
    {
        return rightDir;
    }

    QMatrix4x4 getViewMatrix() const
    {
        QVector3D Ov = modelMatrix * position;
        QVector3D T =  UpDir;
        QVector3D N = (forwardDir).normalized();
        QVector3D k = N * (1/N.length());
        QVector3D i = QVector3D::crossProduct(T, N) * (1 / (QVector3D::crossProduct(T, N)).length());
        QVector3D j = QVector3D::crossProduct(k,i);

        double m03 = -QVector3D::dotProduct(i,Ov);//-(i.x() * Ov.x() + i.y() * Ov.y() + i.z() * Ov.z());
        double m13 = -QVector3D::dotProduct(j,Ov);//-(j.x() * Ov.x() + j.y() * Ov.y() + j.z() * Ov.z());
        double m23 = -QVector3D::dotProduct(k,Ov);//-(k.x() * Ov.x() + k.y() * Ov.y() + k.z() * Ov.z());


        QMatrix4x4 viewMatrix(i.x(), i.y(), i.z(), m03,
                              j.x(), j.y(), j.z(), m13,
                              k.x(), k.y(), k.z(), m23,
                              0,     0,     0,     1);

        return viewMatrix.transposed();
    }

};

struct Material {
    QVector3D albedo;
    QVector3D diffuse;
    float specular;

    Material() : albedo(1,0,0), diffuse(), specular() {}
    Material(const QVector3D &_albedo, const QVector3D &_diffuse, const float &_specular) :
        albedo(_albedo), diffuse(_diffuse), specular(_specular)
    {}
};

struct Light {
public:
    Light(const QVector3D &p, const float &i) : m_position(p), m_intensity(i)
    {}

    void setPosition(const QVector3D &pos) { m_position = pos; }
    QVector3D position() const { return m_position; }

    void setIntensity(const float &intensity) { m_intensity = intensity; }
    float intensity() const { return m_intensity; }

private:
    QVector3D m_position;
    float m_intensity;
};

struct Sphere{
public:
    Sphere(const QVector3D &c, const float &r, const Material &m) : m_position(c), m_radius(r), m_material(m)
    {}
    void setPosition(const QVector3D &pos) { m_position = pos; }
    QVector3D position() const { return m_position; }

    void setRadius(const float &radius) { m_radius = radius; }
    float radius() const { return m_radius; }

    void setMaterial(const Material &material) { m_material = material; }
    Material material() const { return m_material; }

private:
    QVector3D m_position;
    float m_radius;
    Material m_material;

};

#endif // OTHER_H
