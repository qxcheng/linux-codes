#include "MyWidget.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCryptographicHash>
/*
"{
    "key": value
    [
       {},{},
    ]
}"
*/

/*
[
    {},
    {},
    {}
]

*/
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    QByteArray md5  = QCryptographicHash::hash(QByteArray("xueguoliang"), QCryptographicHash::Md5);

    QJsonObject obj;
    obj.insert("username", QString("xueguoliang"));
    obj.insert("password", QString(md5.toHex()));
    obj.insert("male", true);
    obj.insert("age", 36);

    QJsonDocument doc(obj);
    QByteArray json = doc.toJson();
    qDebug() << json;

 //   QJsonDocument doc2 = QJsonDocument::fromJson(json);
  //  QJsonObject obj2 =  doc2.object();
    QJsonObject obj2 = QJsonDocument::fromJson(json).object();

    if(obj2.value("username").toString() == "xueguoliang")
    {
        qDebug() << "yes";
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MyWidget w;
    w.show();

    return app.exec();
}
