#ifndef MENU_H
#define MENU_H

#include "public.h"
#include "person.h"

class Menu
{
public:

    void consumer_menu();
    void seller_menu();
    void rider_menu();
    void admin_menu();

};


template<typename T>
void login_regist_menu(T &t);


/*
void login_regist_menu(Consumer &t);
*/


#endif
