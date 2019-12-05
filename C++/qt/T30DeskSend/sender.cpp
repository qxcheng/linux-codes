#include "sender.h"
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPixmap>
#include <QBuffer>
#include <QHostAddress>
#include <QDateTime>
Sender::Sender(QObject *parent) :
    QObject(parent)
{
    _socket = new QUdpSocket;
}

void Sender::sendFrame(const QByteArray &buf)
{
    // [4096]
    // [frame][5][1][1024]
    // [frame][5][2][1024]
    // [frame][5][3][1024]
    // [frame][5][4][1024]
    // [frame][5][5][2]

    int blockSize = 1024;
    int blockCount = buf.size()/blockSize +
            (buf.size()%blockSize>0?1:0);

    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    for(int i=0; i<blockCount; i++)
    {
        QByteArray block;
        block.append((char*)&timestamp, sizeof(timestamp));
        block.append(char(blockCount));
        block.append(char(i+1));

        block += buf.mid(i*blockSize, blockSize);

        if(_socket->writeDatagram(block, block.size(), QHostAddress("127.0.0.1"), 10001) != block.size())
        {
            qDebug() <<"send error";
        }
    }
}


void Sender::slotSendOneFrame()
{
    // capture desktop
    QPixmap pixmap = QApplication::screens().at(0)->grabWindow(QApplication::desktop()->winId());
    //pixmap.save("../desktop.jpg");


    QBuffer memFile;
    memFile.open(QIODevice::ReadWrite);
    pixmap.save(&memFile, "JPG");
    memFile.close();

    QByteArray buf = memFile.buffer();

    sendFrame(buf);
}
