#include "ImageView.h"

#define USE_DRAW_IMAGE

ImageView::ImageView(QWidget *parent)
    : QLabel(parent), m_size(0, 0)
{
    setScaledContents(true);
    setFocusPolicy(Qt::StrongFocus);
}

ImageView::~ImageView()
{

}

void ImageView::setImage(QImage img)
{
#ifndef USE_DRAW_IMAGE
    setPixmap(QPixmap::fromImage(img));
#else
    m_image = img;
#endif
    update();
}

uchar* ImageView::data()
{
    return m_image.bits();
}

#include <QPainter>

void ImageView::paintEvent(QPaintEvent * e)
{

    QLabel::paintEvent(e);

#ifdef USE_DRAW_IMAGE
    QPainter painter(this);
    QRect rect(0,0, width(), height());
    painter.drawImage(rect, m_image);

#endif
}
