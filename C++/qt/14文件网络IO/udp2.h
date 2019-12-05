#ifndef UDP2_H
#define UDP2_H

#include <QWidget>
#include <QUdpSocket>
class Udp2 : public QWidget
{
    Q_OBJECT
public:
    explicit Udp2(QWidget *parent = 0);
    QUdpSocket* _udp;

signals:

public slots:
    void slotReadyRead();

};

#endif // UDP2_H
