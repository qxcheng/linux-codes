#include "MyWidget.h"
#include <QPainter>
#include <QPixmap>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
}

void MyWidget::paintEvent(QPaintEvent *)
{
    QPixmap pixmap(size());

    QPainter p(&pixmap);

	p.setRenderHint(QPainter::Antialiasing); //边角更精细
  
  //p.translate(100, 100); //坐标变换
  //p.scale();
    
    QTransform transform;
    transform.translate(50,50);
    transform.rotate(30);       //旋转
  //transform.scale(.5, .5);    //缩放
    p.setTransform(transform);	
#if 1
    QTransform transform2;
    transform2.scale(.5, .5);
    p.setTransform(transform2, true);
#endif

    p.drawLine(QPoint(0, 0), QPoint(100, 100));

  //p.translate(-100, -100);
    p.setPen(QPen(Qt::red, 2, Qt::DashLine));
    p.setBrush(Qt::yellow);
    p.setFont(QFont("aaa", 40, 700, true));

    p.drawEllipse(QPoint(95, 333), 50, 50);
    p.drawText(QPoint(300, 50), "Hello world");
  //p.drawPixmap(QPoint(40, 40), QPixmap("../aaa.png"));
  //p.drawRect(QRect(40, 60, 100, 50));
    p.drawRoundRect(QRect(40, 60, 100, 50));

    p.end();

    p.begin(this);
    p.drawPixmap(0, 0, pixmap);

}

void MyWidget::mousePressEvent(QMouseEvent *)
{
    QPixmap pixmap(size());
    QPainter painter(&pixmap);
    render(&painter);
    pixmap.save("../Painter.png");
}

#include <QApplication>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MyWidget w;
    w.show();

    return app.exec();
}
