#include "mywidget.h"
#include <QPainter>
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
}

void MyWidget::slotFrameArrive(QByteArray ba)
{
    _buf = ba;
    update();
}

void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPixmap pixmap;
    pixmap.loadFromData(_buf, "JPG");
    p.drawPixmap(0, 0, pixmap);
}
