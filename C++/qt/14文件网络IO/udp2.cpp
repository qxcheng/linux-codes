#include "udp2.h"
#include <QTimer>
#include <QDateTime>

#include <QLineEdit>

Udp2::Udp2(QWidget *parent) :
    QWidget(parent)
{
    _udp = new QUdpSocket;

    // the address of bind and multicast must be same tpye(IPV4 or IPV6)
    _udp->bind(QHostAddress::AnyIPv4, 10002);

    // join the multicast address (224.0.0.131) for recv mulicast package
    _udp->joinMulticastGroup(QHostAddress("224.0.0.131"));

    connect(_udp, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));

    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, [&](){
        quint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        QString str = QString::number(timestamp);
        _udp->writeDatagram(str.toUtf8(), QHostAddress("127.0.0.1"), 10001);
    });
}

void Udp2::slotReadyRead()
{
    while(_udp->hasPendingDatagrams())
    {
        quint32 datagramSize = _udp->pendingDatagramSize();
        QByteArray buf(datagramSize, 0);
        _udp->readDatagram(buf.data(), buf.size());
        qDebug() << "Udp2" <<buf;
    }
}
