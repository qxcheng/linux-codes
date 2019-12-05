#include "person.h"
#include "mysql.h"
#include "order.h"
#include "food.h"


/* Person class */
Person::Person()
{
    _id         = 0;
    _pwd        = "";
    _name       = "";
    _phone      = "";
    _addr       = "";
    _login_flag = 0;
}


void Person::show_self(string table)/*{{{*/
{
    char sql[128] = {0};
    sprintf(sql, "SELECT * FROM %s WHERE id=%d",table.c_str(), _id);
    Mysql mysql;
    mysql.show_all(sql);
    getchar();
    getchar();
}/*}}}*/

void Person::set_self(string table)/*{{{*/
{
    Mysql mysql;
    string pwd, name, phone, addr;
    char sql[128] = {0};
    cout << "Select which to modify:" << endl;
    cout << "1 password" << endl;
    cout << "2 name" << endl;
    cout << "3 phone" << endl;
    cout << "4 address" << endl;
    int key;
    cin >> key;
    switch(key)
    {
        case 1:
            cout << "Please input new password:" << endl;
            cin >> pwd;
            sprintf(sql, "UPDATE %s SET pwd='%s' WHERE id=%d", table.c_str(),pwd.c_str(),_id);
            if(mysql.handle_mysql(sql))
            {
                cout << "modify ok!" << endl;
                _pwd = pwd;
            }
            else   
            {
                cout << "modify error..." << endl;
            }
            break;
        case 2:
            cout << "Please input new name:" << endl;
            cin >> name;
            sprintf(sql, "UPDATE %s SET name='%s' WHERE id=%d", table.c_str(),name.c_str(),_id);
            if(mysql.handle_mysql(sql))
            {
                cout << "modify ok!" << endl;
                _name = name;
            }
            else
            {
                cout << "modify error..." << endl;
            }
            break;
        case 3:
            cout << "Please input new phone:" << endl;
            cin >> phone;
            sprintf(sql, "UPDATE %s SET phone='%s' WHERE id=%d", table.c_str(),phone.c_str(),_id);
            if(mysql.handle_mysql(sql))
            {
                cout << "modify ok!" << endl;
                _phone = phone;
            }
            else
            {
                cout << "modify error..." << endl;
            }
            break;
        case 4:
            cout << "Please input new address:" << endl;
            cin >> addr;
            sprintf(sql, "UPDATE %s SET addr='%s' WHERE id=%d", table.c_str(),addr.c_str(),_id);
            if(mysql.handle_mysql(sql))
            {
                cout << "modify ok!" << endl;
                _addr = addr;
            }
            else
            {
                cout << "modify error..." << endl;
            }
            break;
        default:
            cout << "wrong choice..." << endl;
            break;
    }

}/*}}}*/
/***** END ******/

/* Seller class *//*{{{*/
Seller::Seller() : Person()
{
    _table = "seller";
}

void Seller::food_manage()
{
    int flag = 1;
    while(flag)
    {
        system("clear");
        cout << "=======Food Menu=====" << endl;
        cout << "1 --> show all food" << endl;
        cout << "2 --> add new food" << endl;
        cout << "3 --> delete food" << endl;
        cout << "4 --> modify food" << endl;
        cout << "5 --> return" << endl;
        cout << "=====================" << endl;
        cout << "Please select:" << endl;
    
        Food food(_id);
        int key, id;
        cin >> key;
        switch(key)
        {
            case 1:
                food.show();
                getchar();
                getchar();
                break;
            case 2:
                food.add();
                break;
            case 3:
                food.del();
                break;
            case 4:
                cout << "input food's id:" << endl;
                cin >> id;
                food.set(id);
                break;
            case 5:
                flag = 0;
                break;
            default:
                cout << "wrong choice" << endl;
                break;
        }
    }
}

void Seller::order_manage()
{
    int id;
    char key;
    Order order;
    Mysql mysql;
    char sql[128] = {0};
    cout << "Order Manage Menu" << endl;
    order.receive_order(_id);
    cout << "Please select a order:" << endl;
    cin >> id;
    cout << "Whether to receive the order:(y/n)" << endl;
    cin >> key;
    if('y' == key)
    {
        sprintf(sql, "UPDATE orderer SET flag2=1 WHERE id=%d",id);
        mysql.handle_mysql(sql);
        cout << "received order ok!" << endl;
    }
    else if('n' == key)
    {
        sprintf(sql, "UPDATE orderer SET flag2=2 WHERE id=%d",id);
        mysql.handle_mysql(sql);
        cout << "rejected order ok!" << endl;
    }
    getchar();
    getchar();
}/*}}}*/
/***** END ******/

/* Consumer class */
Consumer::Consumer() : Person()
{
    _table = "consumer";
}

void Consumer::food_manage()
{
    int food_id, sid;
    int price = 0;
    string foodName;
    char key;
    Order order;
    Mysql mysql;
    mysql.show_seller();
    cout << "Please select a restaurant:" << endl;
    cin >> sid;
    Food food(sid);
    food.show();
    cout << "Please select your food:" << endl;
    do
    {
        cin >> food_id;
        foodName += mysql.get_foodname(food_id);
        price += mysql.get_price(food_id);
        cout << "Whether continue to select:(y/n)" << endl;
        cin >> key;
    }while('y' == key);
    if(price >= 60)
    {
        price -= 20;
    }
    cout << "Order sended:" << foodName << " $" << price << endl;
    getchar();
    getchar();
    order.request_order(sid,_id,foodName,price,_addr);       
}

void Consumer::order_manage()
{
    Mysql mysql;
    char sql[128] = {0};
    cout << "Order Info" << endl;
    cout << "1 history order" << endl;
    cout << "2 incompleted order" << endl;
    cout << "Please select:" << endl;
    int key;
    cin >> key;
    switch(key)
    {
        case 1:
            sprintf(sql, "SELECT * FROM orderer WHERE cid=%d AND flag4=1",_id);
            mysql.show_order(sql);
            break;
        case 2:
            sprintf(sql, "SELECT * FROM orderer WHERE cid=%d AND flag2=2",_id);
            mysql.show_order(sql);
            break;
        default:
            cout << "wrong choice" << endl;
            break;
    }
    getchar();
    getchar();
}
/***** END ******/

/* Rider class */
Rider::Rider() : Person()
{
    _table = "rider";
}

void Rider::_get_order()
{
    int id;
    char sql[128] = {0};
    Mysql mysql;
    Order order;
    order.get_order();
    cout << "Please select a order:" << endl;
    cin >> id;
    sprintf(sql, "UPDATE orderer SET flag3=1, rid=%d WHERE id=%d",_id,id);
    mysql.handle_mysql(sql);
    cout << "get order ok!" << endl;
    cout << "Your food is on the way,please wait..." << endl;
    sleep(5);
    order.finish_order();
    cout << "Your food has arrived!" << endl;
    getchar();
    getchar();
}

void Rider::show_order()
{
    char sql[128] = {0};
    Mysql mysql;
    sprintf(sql, "SELECT * FROM orderer WHERE rid=%d AND flag4=1",_id);
    mysql.show_order(sql);
    getchar();
    getchar();
}

/***** END ******/

/* Admin class */
Admin::Admin() : Person()
{
    _table = "";
}

/***** END ******/



