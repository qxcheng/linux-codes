#include "MyWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QPrinter>

/* QPixmap, QImage, QBitmap, QPicture */

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    QGraphicsLineItem* lineItem;
    QGraphicsTextItem* textItem;
    QGraphicsPixmapItem* pixmapItem;

	//上层组织
    _view = new QGraphicsView(this);
 // _view->setViewport();
 // _view->setBackgroundBrush(Qt::red);
    _view->setScene(_scene = new QGraphicsScene);

	//添加具体数据结构
    _scene->addItem(lineItem = new QGraphicsLineItem(QLineF(QPointF(0, 0), QPointF(100, 100))));
    _scene->addItem(textItem = new QGraphicsTextItem("Hello world"));
    _scene->addItem(pixmapItem = new QGraphicsPixmapItem(QPixmap("../aaa.png")));

	//文字
    QTransform trans;
    trans.rotate(30);
    textItem->setPos(QPointF(200, 300));
    textItem->setTransform(trans);
    textItem->setFont(QFont("aaa", 50, 700, true));

	//图画
    pixmapItem->setPos(100, 100);

    //静态动画
    QGraphicsItemAnimation* animation = new QGraphicsItemAnimation;
    animation->setItem(pixmapItem);

    QTimeLine* timeline = new QTimeLine(3000);
    timeline->setLoopCount(2);

    animation->setTimeLine(timeline);
    animation->setTranslationAt(1, 200, 200);

  //timeline->start();

#if 0
	//定时器
    _timer = new QTimer();
    _timer->setInterval(1000); //1000ms
    connect(_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    _timer->start();
#endif
    //只定时一次
    QTimer::singleShot(1000, this, SLOT(slotTimeout()));
}

//定时槽函数
void MyWidget::slotTimeout()
{
    qDebug() << "Time out";
}

//resize槽函数
void MyWidget::resizeEvent(QResizeEvent *)
{
    // set the size of _view = MyWidget::size
    _view->setGeometry(QRect(QPoint(0, 0), size()));
}

//绘画槽函数
void MyWidget::paintEvent(QPaintEvent *)
{

}

//鼠标点击槽函数
void MyWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::RightButton)
    {
#if 0
        // save the view
        QPixmap pixmap(size());
        QPainter painter(&pixmap);
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);

        _view->render(&painter);
        pixmap.save("../bbb.png");
#endif
#if 0
        print preview
        QPrintPreviewDialog dlg;
        connect(&dlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(slotPaintRequested(QPrinter*)));
        dlg.exec();
#endif
        QPrintDialog dlg;
        connect(&dlg, SIGNAL(accepted(QPrinter*)), this, SLOT(slotPaintRequested(QPrinter*)));
        dlg.exec();
    }
}

//打印槽函数
void MyWidget::slotPaintRequested(QPrinter *printer)
{
    QPainter painter(printer);
    _scene->render(&painter);
    painter.drawText(QPoint(100, 100), "Fuck");
}


int main(int argc, char* argv[])
{
    QApplication app (argc,argv);

    MyWidget w;
    w.showMaximized();
	
    return app.exec();
}
