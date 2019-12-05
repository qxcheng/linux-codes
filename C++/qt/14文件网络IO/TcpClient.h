#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = 0);

    QTcpSocket* _socket;

    QLineEdit* _lineEdit;

signals:

public slots:
    void slotButtonClick();

};

#endif // TCPCLIENT_H
