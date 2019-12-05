#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);

    QByteArray _buf;
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void slotFrameArrive(QByteArray);

};

#endif // MYWIDGET_H
