#ifndef ORDER_H
#define ORDER_H

#include "public.h"

class Order
{
public:
    Order();

    void request_order(int sid, int cid, string food, int price, string addr);
    void receive_order(int sid);
    void get_order();
    void finish_order();

private:
    int _id;
    int _sid;
    int _cid;
    int _rid;
    string _food;
    int _price;
    string _addr;
    int _flag1;    //顾客下单标志
    int _flag2;    //商家接单标志
    int _flag3;    //骑手抢单标志
    int _flag4;    //骑手送达标志

};








#endif
