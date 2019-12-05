
#include "mysql.h"



Mysql::Mysql()
{
    _num_rows = 0;
    mysql_init(&_mysql);
    if( ! mysql_real_connect(&_mysql,"localhost","root","qqq","Eleme",0,NULL,0))
    {
        cout << "mysql connect fail..." << endl;
    }
}

Mysql::~Mysql()
{
    mysql_close(&_mysql);
}

bool Mysql::handle_mysql(const char* query)
{
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        return false;   
    } 
    return true;
}

void Mysql::show_all(const char* sql)
{
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return ;
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    unsigned int num = mysql_num_fields(result);
    MYSQL_ROW row;
    MYSQL_FIELD *field = mysql_fetch_fields(result);
    for(int j=0; j<num; j++)
    {
        cout << field[j].name << "\t";
    }
    cout << endl;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        for(int i=0; i<num; i++)
            cout << row[i] << "\t";
        cout << endl;
    }
    mysql_free_result(result);
//    getchar();
//    getchar();
}

void Mysql::show_seller()
{
    char sql[128] = "select * from seller";
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return ;
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    unsigned int num = mysql_num_fields(result);
    MYSQL_ROW row;
    cout << "id\tname\tphone\taddress" << endl;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        for(int i=0; i<num; i++)
        { 
            if(1 == i)
            {
                continue;
            }
            cout << row[i] << "\t";
        }
        cout << endl;
    }
    mysql_free_result(result);    
//    getchar();
 //   getchar();
}

bool Mysql::show_login(const char *sql)
{
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return false;
    }
    _result = mysql_store_result(&_mysql);
    _num_rows = mysql_num_rows(_result);
    mysql_free_result(_result);
    if(1 == _num_rows)
    { 
        return true;
    }
    else
    {
        return false;    
    }
}

void Mysql::show_order(const char* sql)
{
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return ;
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    unsigned int num = mysql_num_fields(result);
    MYSQL_ROW row;
    cout << "id\tsid\tcid\trid\tfood\tprice\taddress" << endl;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        for(int i=0; i<num; i++)
        { 
            if(7 == i)
            {
                break;
            }
            cout << row[i] << "\t";
        }
        cout << endl;
    }
    mysql_free_result(result);    
//    getchar();
//    getchar();
}

string Mysql::get_addr(int id, string table)
{
    char sql[128] = {0};
    sprintf(sql, "SELECT addr FROM %s WHERE id=%d", table.c_str(), id);
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return "";
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);    
    return string(row[0]); 
}
    
int Mysql::get_price(int id)
{
    char sql[128] = {0};
    sprintf(sql, "SELECT price FROM food WHERE id=%d", id);
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);  
    return atoi(row[0]);
}

string Mysql::get_foodname(int id)
{
    char sql[128] = {0};
    sprintf(sql, "SELECT name FROM food WHERE id=%d", id);
    if(mysql_real_query(&_mysql, sql, strlen(sql)) != 0)
    {
        cout << "read fail!" << endl;
        return "";
    }
    MYSQL_RES *result = mysql_store_result(&_mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);    
    return string(row[0]); 
}
