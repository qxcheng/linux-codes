#ifndef MYSQL_H
#define MYSQL_H

#include "public.h"
#include <mysql/mysql.h>

class Mysql
{
public:
    Mysql();
    ~Mysql();

    bool handle_mysql(const char *query);

    void show_all(const char *query);
    void show_seller();
    bool show_login(const char *query);

    string get_addr(int id, string table);

    void show_order(const char* query);

    int get_price(int id);
    
    string get_foodname(int id);
    
private:
    MYSQL _mysql;
    int _num_rows;
    MYSQL_RES *_result;

};

#endif
