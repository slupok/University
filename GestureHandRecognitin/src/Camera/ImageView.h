#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>

class ImageView : public QLabel
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();

    void setImage(QImage img);
    uchar* data();

protected:
    void paintEvent(QPaintEvent * e) override;

private:
    QPixmap m_pixmap;
    QImage m_image;
    QSize m_size;
};
#endif // IMAGE_VIEW_H
