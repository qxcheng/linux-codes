#include "TcpServer.h"
#include <QHBoxLayout>
#include <QNetworkInterface>
#include <QMessageBox>
#include "ChooseInterface.h"
TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent)
{
    // 创建服务器并监听
    _server = new QTcpServer;

    ChooseInterface dlg;
    dlg.exec();

    QMessageBox::information(NULL,"you select the ip:", dlg._strSelect);

    _server->listen(QHostAddress(dlg._strSelect), 9988);

    // 当有客户端来连接时，调用slotNetConnection方法
    connect(_server, SIGNAL(newConnection()),
            this, SLOT(slotNetConnection()));

    _show = new QTextBrowser;
    QHBoxLayout* lay = new QHBoxLayout(this);
    lay->addWidget(_show);
}

void TcpServer::slotNetConnection()
{
    // 判断是否有未处理的连接
    while(_server->hasPendingConnections())
    {
        // 调用nextPeddingConnection去获得连接的socket
        _socket = _server->nextPendingConnection();

        _show->append("New connection ....");

        // 为新的socket提供槽函数，来接收数据
        connect(_socket, SIGNAL(readyRead()),
                this, SLOT(slotReadyRead()));
    }
}

void TcpServer::slotReadyRead()
{
    // 接收数据，判断是否有数据，如果有，通过readAll函数获取所有数据
    while(_socket->bytesAvailable() > 0)
    {
        _show->append("Data arrived ..... ");
        QByteArray buf = _socket->readAll();
        _show->append(buf);
    }
}
