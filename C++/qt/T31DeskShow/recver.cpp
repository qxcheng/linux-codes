#include "recver.h"
#include <QDebug>
Recver::Recver(QObject *parent) :
    QObject(parent)
{
    _socket = new QUdpSocket;
    _socket->bind(10001);
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
    _timestamp = 0;
}

void Recver::slotDataArrive()
{

    while(_socket->hasPendingDatagrams())
    {

        int size = _socket->pendingDatagramSize();
        QByteArray buf(size, 0);
        _socket->readDatagram(buf.data(), buf.size());

         qint64 timestamp = *(qint64*)buf.data();
         if(timestamp > _timestamp)
         {
             _timestamp = timestamp;
             _blocks.clear();
         }
         else if(timestamp < _timestamp)
         {
             continue;
         }


       // emit this->sigFrameArrive(buf);
        _blocks.append(buf);

            int totalBlockCount = _blocks.at(0).at(8);
            qDebug() << totalBlockCount << _blocks.size();
            if(_blocks.size() == totalBlockCount)
            {
                qDebug()<< "recv frame";

                QByteArray frame;
                for(int i=0; i<_blocks.size(); ++i)
                {
                    frame += _blocks.at(i).mid(10);
                }
                _blocks.clear();
                emit this->sigFrameArrive(frame);
            }

    }
}
