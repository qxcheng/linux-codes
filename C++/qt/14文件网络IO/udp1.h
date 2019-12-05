#ifndef UDP1_H
#define UDP1_H

#include <QWidget>
#include <QUdpSocket>

class Udp1 : public QWidget
{
    Q_OBJECT
public:
    explicit Udp1(QWidget *parent = 0);
    QUdpSocket* _udp;

signals:

public slots:
    void slotReadyRead();
};

#endif // UDP1_H
