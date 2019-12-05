#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QUdpSocket>

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = 0);

    QUdpSocket* _socket;
    void sendFrame(const QByteArray& buf);

signals:

public slots:
    void slotSendOneFrame();

};

#endif // SENDER_H
