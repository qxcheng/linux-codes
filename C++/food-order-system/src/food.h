#ifndef FOOD_H
#define FOOD_H

#include "public.h"

class Food
{
public:
    Food(int id);

    void show();
    void add();
    void del();
    void set(int id);
    

private:
    int _id;
    int _sid;
    string _name;
    string _price;
};












#endif
