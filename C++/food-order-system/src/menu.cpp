#include "menu.h"
#include "person.h"
#include "mysql.h"

template<typename T>
void login_regist_menu(T &t)
{
    system("clear");
    cout << "=========Eleme=======" << endl;
    cout << "1 --> login" << endl;
    cout << "2 --> regist" << endl;
    cout << "3 --> exit" << endl;
    cout << endl;
    cout << endl;
    cout << "=====================" << endl;
    cout << "Please select:" << endl;

    int key;
    cin >> key;
    char sql[64] = {0};
    int id;
    string pwd, name, phone, addr;
    Mysql mysql;
    switch(key)
    {
        case 1:
            cout << "your id:" << endl;
            cin >> id;
            cout << "your password:" << endl;
            cin >> pwd;
            sprintf(sql, "select * from %s where id=%d and pwd='%s'", t._table.c_str(), id, pwd.c_str());
            if( mysql.show_login(sql) )
            {
                t._login_flag = 1;
                cout << "login ok!" << endl;
                t._addr = mysql.get_addr(id,t._table);
                t._id = id;
                t._pwd = pwd;
            }
            else
            {
                cout << "invalid id or password..." << endl;
                getchar();
                getchar();
            }
            break;
        case 2:
            cout << "your id:" << endl;
            cin >> id;
            cout << "your password:" << endl;
            cin >> pwd;
            cout << "your name:" << endl;
            cin >> name;
            cout << "your phone:" << endl;
            cin >> phone;
            cout << "your address:" << endl;
            cin >> addr;
            sprintf(sql, "INSERT INTO %s VALUES(%d,'%s','%s','%s','%s')",t._table.c_str(),id,pwd.c_str(),name.c_str(),phone.c_str(),addr.c_str());
            if( mysql.handle_mysql(sql) )
            {
                cout << "regist ok!" << endl;
                getchar();
                getchar();
            }
            break;
        case 3:
            exit(0);
        default:
            cout << "wrong choice" << endl;
            break;
    }
}


void Menu::consumer_menu()
{
    Consumer consumer;
    while(1)
    {
//cout << "in" << endl;
        login_regist_menu<Consumer>(consumer);
      //  login_regist_menu(consumer);
//cout << "out" << endl;
        while(1 == consumer._login_flag)
        {
            //_login_flag = 0;
            system("clear");
            cout << "=======Consumer======" << endl;
            cout << "1 --> my profile" << endl;
            cout << "2 --> my settings" << endl;
            cout << "3 --> order food" << endl;
            cout << "4 --> my history order" << endl;
            cout << "5 --> exit" << endl;
            cout << "=====================" << endl;
            cout << "Please select:" << endl;

            int key;
            cin >> key;
            switch(key)
            {
                case 1:
                    consumer.Person::show_self(consumer._table);
                    break;
                case 2:
                    consumer.Person::set_self(consumer._table);
                    break;
                case 3:
                    consumer.food_manage();
                    break;
                case 4:
                    consumer.order_manage();
                    break;
                case 5:
                    exit(0);
                default:
                    cout << "wrong choice!" << endl;
                    break;
            }

        }
    }
}

void Menu::seller_menu()
{
    Seller seller;
    while(1)
    {
        login_regist_menu<Seller>(seller);
        while(1 == seller._login_flag)
        {
            //_login_flag = 0;
            system("clear");
            cout << "========Seller=======" << endl;
            cout << "1 --> my profile" << endl;
            cout << "2 --> my settings" << endl;
            cout << "3 --> food menu" << endl;
            cout << "4 --> my order" << endl;
            cout << "5 --> exit" << endl;
            cout << "=====================" << endl;
            cout << "Please select:" << endl;
            int key;
            cin >> key;
            switch(key)
            {
                case 1:
                    seller.Person::show_self(seller._table);
                    break;
                case 2:
                    seller.Person::set_self(seller._table);
                    break;
                case 3:
                    seller.food_manage();
                    break;
                case 4:
                    seller.order_manage();
                    break;
                case 5:
                    exit(0);
                default:
                    cout << "wrong choice!" << endl;
                    break;
            }

        }
    }
}

void Menu::rider_menu()
{
    Rider rider;
    while(1)
    {
        login_regist_menu<Rider>(rider);
        while(1 == rider._login_flag)
        {
            //_login_flag = 0;
            system("clear");
            cout << "=========Rider=======" << endl;
            cout << "1 --> my profile" << endl;
            cout << "2 --> my settings" << endl;
            cout << "3 --> get order" << endl;
            cout << "4 --> my history order" << endl;
            cout << "5 --> exit" << endl;
            cout << "=====================" << endl;
            cout << "Please select:" << endl;
            int key;
            cin >> key;
            switch(key)
            {
                case 1:
                    rider.Person::show_self(rider._table);
                    break;
                case 2:
                    rider.Person::set_self(rider._table);
                    break;
                case 3:
                    rider._get_order();
                    break;
                case 4:
                    rider.show_order();
                    break;
                case 5:
                    exit(0);
                default:
                    cout << "wrong choice!" << endl;
                    break;
            }


        }
    }
}

void Menu::admin_menu()
{
    int id, pwd;
    cout << "your id:" << endl;
    cin >> id;
    cout << "your password:" << endl;
    cin >> pwd;
    if(id == 123456 && pwd == 123456)
    {
        Admin admin;
        char sql[64];
        Mysql mysql;
        int key;
        while(1)
        {
            system("clear");
            cout << "====Administrator====" << endl;
            cout << "1 --> consumer manage" << endl;
            cout << "2 --> seller manage" << endl;
            cout << "3 --> rider manage" << endl;
            cout << "4 --> order manage" << endl;
            cout << "5 --> exit" << endl;
            cout << "=====================" << endl;
            cout << "Please select:" << endl;
            cin >> key;
            switch(key)
            {
                case 1:
                    admin._table = "consumer";
                    sprintf(sql, "SELECT * FROM consumer");
                    mysql.show_all(sql);
                    cout << "Select a consumer id:" << endl;
                    cin >> admin._id;
                    admin.Person::set_self(admin._table);
                    break;
                case 2:
                    admin._table = "seller";
                    sprintf(sql, "SELECT * FROM seller");
                    mysql.show_all(sql);
                    cout << "Select a seller id:" << endl;
                    cin >> admin._id;
                    admin.Person::set_self(admin._table);
                    break;
                case 3:
                    admin._table = "rider";
                    sprintf(sql, "SELECT * FROM rider");
                    mysql.show_all(sql);
                    cout << "Select a rider id:" << endl;
                    cin >> admin._id;
                    admin.Person::set_self(admin._table);
                    break;
                case 4:
                    sprintf(sql, "SELECT * FROM orderer");
                    mysql.show_all(sql);
                    getchar();
                    getchar();
                    //cout << "Select a order id:" << endl;
                    //cin >> _id;
                    break;
                case 5:
                    exit(0);
                default:
                    cout << "wrong choice..." << endl;
                    break;
            }
        }
    }
    else
    {
        cout << "wrong id or password..." << endl;
    }
}
