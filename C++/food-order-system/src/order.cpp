#include "order.h"
#include "mysql.h"

Order::Order()
{
    _id = 0;
    _sid = 0;
    _cid = 0;
    _rid = 0;
    _food = "";
    _price = 0;
    _addr = "";
    _flag1 = 0;
    _flag2 = 0;
    _flag3 = 0;
    _flag4 = 0;
}

void Order::request_order(int sid, int cid, string food, int price, string addr)
{
    char sql[128] = {0};
    sprintf(sql, "INSERT INTO orderer (sid,cid,rid,food,price,addr,flag1,flag2,flag3,flag4) VALUES (%d,%d,0,'%s',%d,'%s',1,0,0,0)",sid,cid,food.c_str(),price,addr.c_str());
    Mysql mysql;
    if( mysql.handle_mysql(sql) )
    {
        cout << "request order ok!" << endl;
    }
    else
    {
        cout << "request order error..." << endl;
    }
}

void Order::receive_order(int sid)
{
    Mysql mysql;
    char sql[128] = {0};
    sprintf(sql, "SELECT * FROM orderer WHERE sid=%d AND flag1=1 AND flag2=0", sid);
    mysql.show_order(sql);
}

void Order::get_order()
{
    Mysql mysql;
    char sql[128] = {0};
    sprintf(sql, "SELECT * FROM orderer WHERE flag1=1 AND flag2=1 AND flag3=0");
    mysql.show_order(sql);
}

void Order::finish_order()
{
    Mysql mysql;
    char sql[128] = {0};
    sprintf(sql, "UPDATE orderer SET flag4=1 WHERE flag1=1 AND flag2=1 AND flag3=1 AND flag4=0");
    mysql.handle_mysql(sql);
}
