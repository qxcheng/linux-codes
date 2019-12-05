#ifndef RECVER_H
#define RECVER_H

#include <QObject>
#include <QUdpSocket>
class Recver : public QObject
{
    Q_OBJECT
public:
    explicit Recver(QObject *parent = 0);

    QUdpSocket* _socket;
     QList<QByteArray> _blocks;

     qint64 _timestamp;

signals:
    void sigFrameArrive(QByteArray ba);



public slots:
    void slotDataArrive();

};

#endif // RECVER_H
