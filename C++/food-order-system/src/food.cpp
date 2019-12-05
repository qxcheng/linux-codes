#include "food.h"
#include "mysql.h"

Food::Food(int sid)
{
    _id = 0;
    _sid = sid;
    _name = "";
    _price = "";
}

void Food::show()
{
    Mysql mysql;
    char sql[128] = {0};
    sprintf(sql, "SELECT * FROM food WHERE sid=%d", _sid);
    mysql.show_all(sql);
}

void Food::add()
{
    Mysql mysql;
    char sql[128] = {0};
    int price;
    string name;
    cout << "Please input food's name:" << endl;
    cin >> name;
    cout << "Please input food's price:" << endl;
    cin >> price;
    sprintf(sql, "INSERT INTO food (sid,name,price) VALUES (%d, '%s', %d)", _sid,name.c_str(),price);
    if( mysql.handle_mysql(sql) )
    {
        cout << "add ok!" << endl;
    }
    else
    {
        cout << "add error..." << endl;
    }
}

void Food::del()
{
    Mysql mysql;
    char sql[128] = {0};
    int id;
    cout << "Please input food's name to be deleted:" << endl;
    cin >> id;
    sprintf(sql, "DELETE FROM food WHERE id=%d", id);
    if( mysql.handle_mysql(sql) )
    {
        cout << "delete ok!" << endl;
    }
    else
    {
        cout << "delete error..." << endl;
    }
}

void Food::set(int id)
{
    Mysql mysql;
    char sql[128] = {0};
    int key;
    cout << "Select which to modify:" << endl;
    cout << "1 food's name" << endl;
    cout << "2 food's price" << endl;
    cin >> key;
    switch(key)
    {
        case 1:
            cout << "input a new name:" << endl;
            cin >> _name;
            sprintf(sql, "UPDATE food SET name='%s' WHERE id=%d",_name.c_str(),id);
            if( mysql.handle_mysql(sql) )
            {
                cout << "modify ok!" << endl;
            }
            cout << "modify error..." << endl;
            break;
        case 2:
            cout << "input a new price:" << endl;
            cin >> _price;
            sprintf(sql, "UPDATE food SET price='%s' WHERE id=%d",_price.c_str(),id);
            if( mysql.handle_mysql(sql) )
            {
                cout << "modify ok!" << endl;
            }
            cout << "modify error..." << endl;
            break;
        default:
            cout << "wrong choice..." << endl;
            break;
    }
}
