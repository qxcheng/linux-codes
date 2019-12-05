#include "udp1.h"
#include <QTimer>
#include <QDateTime>
Udp1::Udp1(QWidget *parent) :
    QWidget(parent)
{
    // 创建udpsocket，并连接槽函数，用来接收数据
    _udp = new QUdpSocket;
    _udp->bind(10001);
    connect(_udp, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));

    // 使用定时器来定时发送时间戳
    QTimer* timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, [&](){
        quint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        QString str = QString::number(timestamp);
#if 0
        // 普通UDPsocket发送
        _udp->writeDatagram(str.toUtf8(), QHostAddress("127.0.0.1"), 10002);
#else
        // 广播发送，注意：QHostAddress::Broadcast是255.255.255.255, 192.168.6.255
     //   _udp->writeDatagram(str.toUtf8(), QHostAddress::Broadcast, 10002);

        // multicast, 224.0.0.1~224.0.0.255 is multicast address of LAN
        _udp->writeDatagram(str.toUtf8(), QHostAddress("224.0.0.131"), 10002);
#endif
    });
}

void Udp1::slotReadyRead()
{
    while(_udp->hasPendingDatagrams())
    {
        quint32 datagramSize = _udp->pendingDatagramSize();
        QByteArray buf(datagramSize, 0);
        _udp->readDatagram(buf.data(), buf.size());
        qDebug() <<"Udp1"<< buf;
    }
}
