
#include <QApplication>
#include "Widget05.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "Contact.h"

int main(int argc, char*argv[])
{
    QApplication app(argc, argv);

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

    Widget05 w;
    w.show();
    return app.exec();
}
