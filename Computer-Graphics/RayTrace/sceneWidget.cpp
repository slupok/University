#include "sceneWidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
#include <QMatrix4x4>

SceneWidget::SceneWidget(QWidget *parent)
    :QLabel(parent), m_isPressed(false), m_lastPressedPos()
{
    setScaledContents(true);
    setFocusPolicy(Qt::StrongFocus);
}

void SceneWidget::setRayTrace(RayTrace *rt)
{
   m_rt = rt;
   m_rt->setImageSize(1000, 800);
   setImage(m_rt->draw());
}

void SceneWidget::setImage(const QPixmap &pixmap)
{
    setPixmap(pixmap);
    update();
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPressedPos = event->pos();
    m_isPressed = true;
    m_rt->setImageSize(1000/5, 800/5);
}

void SceneWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    m_rt->setImageSize(1000, 800);
    setImage(m_rt->draw());
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_isPressed)
        return;

    QPoint currentPos = event->pos();
    if(m_lastPressedPos.isNull())
        m_lastPressedPos = currentPos;

    QPoint offset = currentPos - m_lastPressedPos;
    float xOffset = offset.x();
    float yOffset = offset.y();

    m_rt->cameraRotate(yOffset, xOffset, 0);
    setImage(m_rt->draw());

    float c = 0.006f;

    QMatrix4x4 yRotateMatrix(qCos(xOffset*c), 0, -qSin(xOffset*c), 0,
                                           0, 1,                0, 0,
                             qSin(xOffset*c), 0,  qCos(xOffset*c), 0,
                                           0, 0,                0, 1);

    QMatrix4x4 xRotateMatrix(1,                0,                 0, 0,
                             0, qCos(-yOffset*c), -qSin(-yOffset*c), 0,
                             0, qSin(-yOffset*c),  qCos(-yOffset*c), 0,
                             0,                0,                 0, 1);



    m_lastPressedPos = currentPos;
}

void SceneWidget::keyPressEvent(QKeyEvent *event)
{
    QVector3D vec(0,0,0);

    if(event->key() == Qt::Key_W)
        vec += QVector3D(0,0,1);
    else if(event->key() == Qt::Key_S)
        vec += QVector3D(0,0,-1);

    if(event->key() == Qt::Key_A)
        vec += QVector3D(1,0,0);
    else if(event->key() == Qt::Key_D)
        vec += QVector3D(-1,0,0);

    vec.normalize();

    QMatrix4x4 translateMatrix(1, 0, 0, -vec.x()*0.5,
                               0, 1, 0, 0,
                               0, 0, 1, -vec.z()*0.5,
                               0, 0, 0, 1);

    if(event->key() == Qt::Key_Control)
       m_rt->setImageSize(1000, 800);
    else
       m_rt->setImageSize(1000/5, 800/5);

    //m_rt->setCameraModelMatrix(translateMatrix.transposed());
    m_rt->cameraMove(vec);
    setImage(m_rt->draw());
}

void SceneWidget::keyReleaseEvent(QKeyEvent *event)
{

    //m_rt->setImageSize(1280, 720);
    //setImage(m_rt->draw());
}

