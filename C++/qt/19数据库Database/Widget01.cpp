#include "Widget01.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

#include <QDebug>
Widget01::Widget01(QWidget *parent) :
    QWidget(parent)
{
    /* QSQLITE QODBC QPLSQL */
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("iamxgl");
    db.setDatabaseName("d0718");
    bool bRet = db.open();
    if(bRet == false)
    {
        qDebug() << "error open database" << db.lastError().text();
        exit(0);
    }
    qDebug() << "open database success";

 //   db.exec("insert into tuser (username, password, gender) values('xueguoliang', 'xxdfsdf', 1)");
    QSqlQuery query = db.exec("select * from d0718");
   // int size = query.size();

    bool b = query.first();
    while(b)
    {
 //       QSqlResult* result = query.result();
  //      QVector<QVariant> values =  result->boundValues();
     //   qDebug() << values;

        b = query.next();
    }

}
