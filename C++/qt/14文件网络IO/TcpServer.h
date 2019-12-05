#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextBrowser>

class TcpServer : public QWidget
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = 0);

    QTcpServer* _server;
    QTcpSocket* _socket;

    QTextBrowser* _show;

signals:

public slots:
    void slotNetConnection();
    void slotReadyRead();

};

#endif // TCPSERVER_H
