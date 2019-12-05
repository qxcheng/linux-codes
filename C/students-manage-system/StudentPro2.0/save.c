#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "save.h"


/* read data from database */
void read_data(const char* sql)
{
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    if(mysql_real_query(&mysql,sql,strlen(sql)) !=0)
    {
        printf("read fail!\n");
        mysql_close(&mysql);
        return ;
    }
    MYSQL_RES *result = mysql_use_result(&mysql);
    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        for(int i=0; i<num_fields; i++)
            printf("%s\t",row[i]);  
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(&mysql);
}

/* save data to database */
void save_data(const char* sql)
{    
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    if(mysql_real_query(&mysql,sql,strlen(sql)) !=0)
    {
        printf("save data fail!\n");
        mysql_close(&mysql);
        return ;
    }
    mysql_close(&mysql);
//    printf("save data successful!\n");
}

/* change no of students by all three grades */
void no_sort()
{
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    char sql[100];
    char temp[30];
    sprintf(temp,"set @var = 0");
    sprintf(sql,"update STU set no= (@var := @var+1) order by (math+c+chinese) desc");
    mysql_real_query(&mysql,temp,strlen(temp));
    if(mysql_real_query(&mysql,sql,strlen(sql)) != 0)
        printf("sort fail!\n");
    mysql_close(&mysql);
    return ;
}
