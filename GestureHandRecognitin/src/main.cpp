
#include <QApplication>

#include "Camera/ImageView.h"
#include "GestureRecognition/gestureRecognition.h"
//#include "Camera/camera.h"

#include <QDebug>
#include <QObject>
#include <QFile>
#include <QHBoxLayout>
//#include "GestureRecognition/ImageProcessing/imageProcessing.h"

//#include "GestureRecognition/ImageProcessing/ipContext.h"
//#include "GestureRecognition/ImageProcessing/OpenCL/opencl_context.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int w = 1920 / 2;
    int h = 1080 / 2;

    QWidget mainWidget;
    mainWidget.resize(w, h*2);

    ImageView *frameView = new ImageView(&mainWidget);
    frameView->setGeometry(0,0, mainWidget.size().width(), mainWidget.size().height());
    frameView->setObjectName(QString::fromUtf8("frame"));
    frameView->setAutoFillBackground(true);

    ImageView *maskView = new ImageView(&mainWidget);
    maskView->setGeometry(0,0, mainWidget.size().width(), mainWidget.size().height());
    maskView->setObjectName(QString::fromUtf8("mask"));
    maskView->setAutoFillBackground(true);

    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);
    layout->addWidget(frameView);
    layout->addWidget(maskView);

    mainWidget.setWindowTitle(QObject::tr("Gesture Hand Recognition"));
    mainWidget.show();

    GestureRecognition *rg = new GestureRecognition();
    rg->setCaptureView(frameView);
    rg->setMaskView(maskView);
    rg->startGR();

    return a.exec();
}
