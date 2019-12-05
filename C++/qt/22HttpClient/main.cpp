#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager* manager = new QNetworkAccessManager;

#if 0
    QNetworkRequest req;
    req.setUrl(QUrl("http://ds-project.cn"));
    manager->get(req);
    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply){
            if(reply->error() != QNetworkReply::NoError)
            {
                qDebug() << "Error:" << reply->errorString();
                return;
            }
            QByteArray buf = reply->readAll();
            qDebug() << "OK:"<< buf;
    });
#endif

#if 0
    QNetworkRequest req;
    req.setUrl(QUrl("http://127.0.0.1/cgi-bin/test.cgi"));
    manager->get(req);
    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply){
            if(reply->error() != QNetworkReply::NoError)
            {
                qDebug() << "Error:" << reply->errorString();
                return;
            }
            QByteArray buf = reply->readAll();
            qDebug() << "OK:"<< buf;
    });
#endif

    QJsonObject obj;
    obj.insert("username", QString("xueguoliang"));
    obj.insert("password", QString("xueguoliang's password"));

    QNetworkRequest req;
    req.setUrl(QUrl("http://127.0.0.1/cgi-bin/qt-test/qt-test.cgi"));
    manager->post(req, QJsonDocument(obj).toJson());
    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply){
            if(reply->error() != QNetworkReply::NoError)
            {
                qDebug() << "Error:" << reply->errorString();
                return;
            }
            QByteArray buf = reply->readAll();
            qDebug() << "OK:"<< buf;
    });


    return app.exec();
}
