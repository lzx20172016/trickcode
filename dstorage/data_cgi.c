

#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include"upload_file.h"
#include "fcgi_stdio.h"
#include"fcgi_config.h"
#include"util.h"
#include"./make_log.h"
extern char**environ;

#define FCGI "fcgi1"
#define FCGI_DATA "data"

#define VALUE_LEN 128

int move_2_public_list(char*file_id)
{
    int retn=0;
   redisContext *conn=NULL;
  conn=rop_connectdb_nopwd(REDIS_IP,REDIS_PORT);
 if(conn==NULL)
 {
    LOG(FCGI,FCGI_DATA,"conn redis error",file_id);
    retn=-1;
    goto END;
 } 

 //将文件拷贝到public list中
 rop_list_push(conn,FILE_PUBLIC_LIST,file_id);
 //将文件的引用计数加1
    retn=rop_hincrement_one_field(conn,FILE_REFERENCE_COUNT_HASH,file_id,1);

    if(retn==-1)
    {
        LOG(FCGI,FCGI_DATA," add pv %s error",file_id);
        goto END;
    }

    //将文件分享状态设置为1
    retn=rop_hash_set(conn,FILEID_SHARED_STATUS_HASH,file_id,"1");
    if(retn==-1)
    {
        LOG(FCGI,FCGI_DATA," hset %s %s error",FILEID_SHARED_STATUS_HASH,file_id);
        goto END;
    }




END:
 if(conn!=NULL)
 {
        rop_disconnect(conn);
 }
 return retn;
}
void increase_pv(char*file_id)
{
    int ret=0;
    redisContext*conn=NULL;
    conn=rop_connectdb_nopwd("127.0.0.1","6379");
    if(conn==NULL)
    {
        LOG(FCGI,FCGI_DATA,"conn redis server errror");
        return ;
    }

    ret=rop_hincrement_one_field(conn,FILEID_PV_HASH,file_id,1);
    if(ret==-1)
    {
        LOG(FCGI,FCGI_DATA,"rop_hincrement_one_field error add pv %s error",file_id);
        return ;
    }

    rop_disconnect(conn);

}


int main ()
{
    int retn=0;
    char *query_string=NULL;
    //   int count = 0;
    while (FCGI_Accept() >= 0) {
        //	int len;
        char cmd[VALUE_LEN]={0};
        char count[VALUE_LEN]={0};
        char fromId[VALUE_LEN]={0};
        char file_id[VALUE_LEN]={0};
        char user[VALUE_LEN]={0};
        char *json_str=NULL;
        // char **initialEnv = environ;
        printf("Content-type: text/html\r\n"
                "\r\n");

        query_string=getenv("QUERY_STRING");
        retn=query_parse_key_value(query_string,"cmd",cmd,NULL);
        LOG(FCGI,FCGI_DATA,"query_string=[%s]",query_string);
        
        if(strcmp(cmd,"newFile")==0)
        {
            //住界面查询数据的业务
            query_parse_key_value(query_string,"fromId",fromId,NULL);
            query_parse_key_value(query_string,"count",count,NULL);
            query_parse_key_value(query_string,"user",user,NULL);
            LOG(FCGI,FCGI_DATA,"fromd=[%s],count=%s,user=%s",fromId,count,user);

            //查询数据库
#if 0
            char*json_str=(char*)malloc(4096);
            memset(json_str,0,4096);
            FILE *fp=fopen("json_test_data.json","r");
            fread(json_str,4096,1,fp);
            fclose(fp);
#endif

            retn=select_files(atoi(fromId),atoi(count),user,cmd,&json_str);
//            retn=select_files(atoi(fromId),1,&json_str);
            if(retn!=0)
            {
            LOG(FCGI,FCGI_DATA,"select files error");
                goto END;
            }
            //将数据打印给前端
            LOG(FCGI,FCGI_DATA,"newFile:json_str=[\n%s\n]",json_str);
             printf("%s",json_str);
        }else if(strcmp(cmd,"increase")==0)
        {
            //增加点击量业务
            //得到fileid
            query_parse_key_value(query_string,"fileId",file_id,NULL);
            //fileid中的%2F
            str_replace(file_id,"%2F","/");
            LOG(FCGI,FCGI_DATA,"increase:fileid=[%s]",file_id);
            //更改FILEID_PV_HASH field 字段

            increase_pv(file_id);
        }
        else if(strcmp(cmd,"shared")==0)
        {
            //文件被点击分享
            query_parse_key_value(query_string,"fileId",file_id,NULL);
            query_parse_key_value(query_string,"user",user,NULL);
            //fileid中的%2F--->/
            str_replace(file_id,"%2F","/");
            //将文件拷贝到public list表中
            move_2_public_list(file_id);
        }
        else if(strcmp(cmd,"shareFile")==0)
        {
           //共享文件页面查询数据的业务 
            query_parse_key_value(query_string,"fromId",fromId,NULL);
            query_parse_key_value(query_string,"count",count,NULL);
            query_parse_key_value(query_string,"user",user,NULL);
            LOG(FCGI,FCGI_DATA,"fromd=[%s],count=%s,user=%s",fromId,count,user);
            retn=select_files(atoi(fromId),atoi(count),user,cmd,&json_str);
//            retn=select_files(atoi(fromId),1,&json_str);
            if(retn!=0)
            {
            LOG(FCGI,FCGI_DATA,"select files error");
                goto END;
            }
            //将数据打印给前端
            LOG(FCGI,FCGI_DATA,"shareFile:json_str=[\n%s\n]",json_str);
             printf("%s",json_str);

        }
        //查询数据库
END:
            if(json_str!=NULL)
            {
                free(json_str);
                json_str=NULL;
            }
    } /* while */

    return 0;
}
