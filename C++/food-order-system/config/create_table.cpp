#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


int main()
{
    MYSQL _mysql;

    mysql_init(&_mysql);
    if( ! mysql_real_connect(&_mysql,"localhost","root","qqq","Eleme",0,NULL,0))
    {
        printf("connect fail!\n");
    }

    char query[1024] = {0};

    //consumer table
    strcpy(query, "CREATE TABLE consumer (id int(4) primary key, pwd varchar(32), name varchar(32), phone varchar(32), addr varchar(32));");
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        printf("exec error\n");
    }
    memset(query, sizeof(query), 0);

    //seller table
    strcpy(query, "CREATE TABLE seller (id int(4) primary key, pwd varchar(32), name varchar(32), phone varchar(32), addr varchar(32));");
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        printf("exec error\n");
    }
    memset(query, sizeof(query), 0);

    //rider
    strcpy(query, "CREATE TABLE rider (id int(4) primary key, pwd varchar(32), name varchar(32), phone varchar(32), addr varchar(32));");
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        printf("exec error\n");
    } 
    memset(query, sizeof(query), 0);

    //food
    strcpy(query, "CREATE TABLE food (id int(4) primary key auto_increment, sid int(4), name varchar(32), price int(4));");
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        printf("exec error\n");
    } 
    memset(query, sizeof(query), 0);

    //order
    strcpy(query, "CREATE TABLE orderer (id int(4) primary key auto_increment, sid int(4), cid int(4), rid int(4), food varchar(32), price int(4), addr varchar(32), flag1 int(4), flag2 int(4), flag3 int(4), flag4 int(4));");
    if(mysql_real_query(&_mysql, query, strlen(query)) != 0)
    {
        printf("exec error\n");
    } 
    memset(query, sizeof(query), 0);

    mysql_close(&_mysql);

}
