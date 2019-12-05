#include "menu.h"

int main()
{
    Menu menu;
    while(1)
    {
        system("clear");
        cout << "========Eleme========" << endl;
        cout << "1 --> consumer" << endl;
        cout << "2 --> seller" << endl;
        cout << "3 --> rider" << endl;
        cout << "4 --> administrator" << endl;
        cout << "5 --> exit" << endl;
        cout << "=====================" << endl;
        cout << "Please select:" << endl;
        int key;
        cin >> key;
        switch(key)
        {
            case 1:
                menu.consumer_menu();
                break;
            case 2:
                menu.seller_menu();
                break;
            case 3:
                menu.rider_menu();
                break;
            case 4:
                menu.admin_menu();
                break;
            case 5:
                exit(0);
                break;
            default:
                cout << "wrong choice..." << endl;
                break;
        }
    }
}/*}}}*/

