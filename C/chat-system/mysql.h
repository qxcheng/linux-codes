#ifndef __MYSQL_H__
#define __MYSQL_H__


int findMaxUsrId();

int loginCheck(int usrId, char *usrPwd);

void saveUsrInfo(int usrId, char *usrPwd, char *usrName);

void findUsrName(int usrId, char *usrName);

#endif
