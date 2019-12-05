#include "public.h"
#include "mysql.h"
#include <mysql/mysql.h>

int findMaxUsrId()/*{{{*/
{
    int maxid = -1;
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","qqq","OICQ",0,NULL,0))
    {
        printf("connect mysql failed.\n");
        return -1;
    }
    
    char *sql = "SELECT MAX(usrId) FROM usrinfo";
    if(mysql_real_query(&mysql,sql,strlen(sql)) != 0)
    {
        printf("find max userid failed.\n");
        return -1;
    }
    MYSQL_RES *ret = mysql_use_result(&mysql);
    MYSQL_ROW row;
    if(row = mysql_fetch_row(ret))
    {
        maxid = atoi(row[0]);
    }
    mysql_free_result(ret);
    mysql_close(&mysql);
    return maxid;
}/*}}}*/

int loginCheck(int usrId, char *usrPwd)
{
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","qqq","OICQ",0,NULL,0))
    {
        printf("connect mysql failed.\n");
    }

    char sql[128];
    sprintf(sql, "SELECT * FROM usrinfo WHERE usrId=%d AND usrPwd='%s'",usrId,usrPwd);
    if(mysql_real_query(&mysql,sql,strlen(sql)) != 0)
    {
        printf("find error\n");
        return 0;
    }
    MYSQL_RES *ret = mysql_use_result(&mysql);
    MYSQL_ROW row;
    if((row = mysql_fetch_row(ret)) == NULL)
    {
        return 0;
    }
    mysql_free_result(ret);
    mysql_close(&mysql);
    return 1;
}

void saveUsrInfo(int usrId, char *usrPwd, char *usrName)/*{{{*/
{
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","qqq","OICQ",0,NULL,0))
    {
        printf("connect mysql failed.\n");
    }

    char sql[128];
    sprintf(sql, "INSERT INTO usrinfo values(%d,'%s','%s')",usrId,usrPwd,usrName);
    if(mysql_real_query(&mysql,sql,strlen(sql)) != 0)
    {
        printf("save user info failed.\n");
        return ;
    }
    mysql_close(&mysql);
}/*}}}*/

//find user's name by ID    
void findUsrName(int usrId, char *usrName)
{
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","qqq","OICQ",0,NULL,0))
    {
        printf("connect mysql failed.\n");
        return ;
    }
    char sql[128];
    sprintf(sql, "SELECT usrName FROM usrinfo WHERE usrID=%d", usrId);
    if(mysql_real_query(&mysql,sql,strlen(sql)) != 0)
    {
        printf("find username failed.\n");
        return ;
    }
    MYSQL_RES *ret = mysql_use_result(&mysql);
    MYSQL_ROW row;
    if(row = mysql_fetch_row(ret))
    {
        strcpy(usrName, row[0]);
    }
    mysql_free_result(ret);
    mysql_close(&mysql);
}
