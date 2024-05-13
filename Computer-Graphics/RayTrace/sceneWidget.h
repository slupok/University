#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H
#include <QLabel>
#include <QMatrix4x4>
#include "RayTrace.h"

class SceneWidget : public QLabel
{
public:
    explicit SceneWidget(QWidget *parent = nullptr);
    void setRayTrace(RayTrace *rt);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void setImage(const QPixmap &pixmap);

private:
    bool m_isPressed;
    QPoint m_lastPressedPos;

    RayTrace *m_rt;
};

#endif // SCENEWIDGET_H
