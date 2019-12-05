#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

#include <QPrintPreviewDialog>  // preview
#include <QPrintDialog>         // print

// display scene
#include <QGraphicsView>

#include <QTimer>
#include <QDateTime>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    QGraphicsScene* _scene;  // data model
    QGraphicsView* _view;   // show the data model

    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);

    QTimer* _timer;
signals:

public slots:
    void slotPaintRequested(QPrinter*);
    void slotTimeout();

};

#endif // MYWIDGET_H
