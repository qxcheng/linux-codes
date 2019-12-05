#ifndef PERSON_H
#define PERSON_H

#include "public.h"

class Person
{
public:
    Person();

    void show_self(string table);    
    void set_self(string table); 

public:
    int _id;
    string _pwd;
    string _name;
    string _phone;
    string _addr;
public:
    int _login_flag;
    
};

class Seller : public Person
{
public:
    Seller();
    void menu();

    void food_manage();
    void order_manage();

public:
    string _table;
};

class Consumer : public Person
{
public:
    Consumer();
    void menu();

    void food_manage();
    void order_manage();
     

public:
    string _table;
};

class Rider : public Person
{
public:
    Rider();
    void menu();
    
    void _get_order();
    void show_order();
public:
    string _table;
};

class Admin : public Person
{
public:
    Admin();
    void menu();


public:
    string _table;
};



#endif
