#include "Widget02.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
Widget02::Widget02(QWidget *parent) :
    QWidget(parent)
{
    // QSqlTableModel: 数据表对应的数据结构
    QSqlTableModel model;
    model.setTable("tuser");
    //model.setFilter("username<>'newuser'");
    model.select();  // exec query
    int ret = model.rowCount();

    // read data from database
    for(int i=0; i<ret; ++i)
    {
       QSqlRecord record = model.record(i);
       for(int j=0; j<record.count(); j++)
       {
           qDebug() << record.value(j);
       }
    }

    // update data to database
 //   model.setData(model.index(0, 1), "xxxxxx");
  //  model.submitAll();

    // insert data to database
    QSqlRecord record = model.record();
//    record.setValue("id", );
    record.setValue("username", "newuser1");
    record.setValue("password", "newpassword");
    record.setValue("gender", 1);
    model.insertRecord(-1, record);
    model.submitAll();

}
