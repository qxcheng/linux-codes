
#include <stdio.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcessEnvironment>


int main(int argc, char* argv[])
{
    printf("Content-type:text/html\n\n");
    QCoreApplication app(argc, argv);

    QProcessEnvironment envir = 
        QProcessEnvironment::systemEnvironment();

    int contentLen = envir.value("CONTENT_LENGTH").toInt();
    QTcpSocket socket;

    socket.setSocketDescriptor(STDIN_FILENO, 
            QTcpSocket::ConnectedState,
            QTcpSocket::ReadOnly);

    QByteArray buf;
    
    QObject::connect(&socket, &QTcpSocket::readyRead, [&](){
                buf += socket.readAll();
                if(buf.size()  == contentLen)
                {
                    QJsonDocument doc = QJsonDocument::fromJson(buf);
                    QJsonObject obj = doc.object();
#if 1
                    QString username = obj.value("username").toString();
                    QString password = obj.value("password").toString();
                    printf("username is %s; password is %s", 
                        username.toUtf8().data(), password.toUtf8().data());
#else
                    QSqlTableModel model;
                    model.setTable("tuser");
                    model.setFilter("username='xx'");
                    model.select();
                    if(model.rowCount() == 1)
                        printf("Login success");
                    else
                        printf("Login Error");
#endif
                    app.quit();
                }
            });

    return app.exec();
}

