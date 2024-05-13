#include "sceneWidget.h"
#include "RayTrace.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWidget;
    mainWidget.resize(800, 600);

    SceneWidget *scene = new SceneWidget(&mainWidget);
    scene->setGeometry(0,0, mainWidget.size().width(), mainWidget.size().height());
    scene->setObjectName(QString::fromUtf8("scene"));
    scene->setAutoFillBackground(true);


    mainWidget.setWindowTitle(QObject::tr("RayTrace"));
    mainWidget.show();

    RayTrace *rt = new RayTrace();
    scene->setRayTrace(rt);

    return a.exec();
}
