#include"upload_file.h"
#include"../util.h"
#include"cJSON.h"
#include"../make_log.h"
#define FCGI "fcgi2saveone"
#define FCGI_PROC "upload_file"
int save_one_file(char*filename)
{
    char file_id[FILE_ID_LEN]={0};
    redisContext *conn=NULL;
    time_t now;
    char time_str[TIME_STR_MAX]={0};
    int ret=0;

    //将文件存入fastdf中-->fileid
    ret=fdfs_upload_by_filename(filename,file_id);
    if(ret==-1)
    {
        LOG(FCGI,FCGI_PROC,"fdfs_upload_by_filename error");
        goto END;
    }
        LOG(FCGI,FCGI_PROC,"fdfs_upload_by_filename fild_id:%s\n",file_id);

    //将fieild-->FILE——INFO——LIST
    conn=rop_connectdb_nopwd("127.0.0.1","6379");
    if(conn==NULL)
    {
        LOG(FCGI,FCGI_PROC,"rop_connectdb_nopwd is error.\n");
        goto END;
    }
    ret=rop_list_push(conn,FILE_USER_LIST,file_id);
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"push %s to %s error\n",file_id,FILE_USER_LIST);
        goto END;
    }
        LOG(FCGI,FCGI_PROC,"push %s to %s success\n",file_id,FILE_USER_LIST);
    //存入文件属性hash中
    //文件名
    ret=rop_hash_set(conn,FILEID_NAME_HASH,file_id,filename);
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_NAME_HASH);
        goto END;
    }

    now=time(NULL);//获得当前系统时间
    strftime(time_str,TIME_STR_MAX,"%Y-%m-%d %H:%M:%S",localtime(&now));
    //创建时间
    ret=rop_hash_set(conn,FILEID_TIME_HASH,file_id,time_str);
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_TIME_HASH);
        goto END;
    }
    //文件所属用户名
    ret=rop_hash_set(conn,FILEID_USER_HASH,file_id,"ICE");
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_USER_HASH);
        goto END;
    }

    //点击量
    ret=rop_hash_set(conn,FILEID_PV_HASH,file_id,"0");
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_PV_HASH);
        goto END;
    }

    char suffix[16]={0};
    get_file_suffix(filename,suffix);


    //文件的类型
    ret=rop_hash_set(conn,FILEID_TYPE_HASH,file_id,"2");
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_TYPE_HASH);
        goto END;
    }

    char file_url[FILE_URL_LEN]={0};
    //拼接一个URL
    strcat(file_url,"http://");
    strcat(file_url,STORAGE_IP);
    strcat(file_url,"/");
    strcat(file_url,file_id);
    //文件URL
    ret=rop_hash_set(conn,FILEID_URL_HASH,file_id,file_url);
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_PV_HASH);
        goto END;
    }
    //文件的分享状态
    ret=rop_hash_set(conn,FILEID_SHARED_STATUS_HASH,file_id,"0");
    if(ret!=0)
    {
        LOG(FCGI,FCGI_PROC,"hset %s to %s error\n",file_id,FILEID_SHARED_STATUS_HASH);
        goto END;
    }
        LOG(FCGI,FCGI_PROC,"hset %s to %s success\n",file_id,FILEID_SHARED_STATUS_HASH);
    rop_disconnect(conn);
END:
    return 0;

}


int select_files(int fromId,int count,char**out)
{
    int retn=0;
    char*p=NULL;
    redisContext*conn=NULL;
    RVALUES file_id_array=NULL;
    int array_len=0;
    int i=0;

    cJSON *root=NULL;
    cJSON*array=NULL;


    conn=rop_connectdb_nopwd("127.0.0.1","6379");
    if(conn==NULL)
    {
        retn=-1;
        LOG(FCGI,FCGI_PROC,"connect redis server error");
        goto END;
    }

    file_id_array=malloc(count*VALUES_ID_SIZE);
    memset(file_id_array,0,count*VALUES_ID_SIZE);

    retn=rop_range_list(conn,FILE_USER_LIST,fromId,fromId+count-1,file_id_array,&array_len);
    if(retn!=0)
    {
        LOG(FCGI,FCGI_PROC,"range list error");
        goto END;
    }

    //创建一个json root object
    root=cJSON_CreateObject();
    //创建一个array[]
    array=cJSON_CreateArray();


        LOG(FCGI,FCGI_PROC,"array_len:%d",array_len);

    for(i=0;i<array_len;i++)
    {
        char name[VALUES_ID_SIZE]={0};
        char time[VALUES_ID_SIZE]={0};
        char user[VALUES_ID_SIZE]={0};
        char pv[VALUES_ID_SIZE]={0};
        char type[VALUES_ID_SIZE]={0};
        char pic_url[VALUES_ID_SIZE]={0};
        char url[VALUES_ID_SIZE]={0};
        char share_status[VALUES_ID_SIZE]={0};
        cJSON*item=cJSON_CreateObject();
        //id
        cJSON_AddStringToObject(item,"id",file_id_array[i]);
        LOG(FCGI,FCGI_PROC,"id=[%s]",file_id_array[i]);

        //kind
        cJSON_AddNumberToObject(item,"kind",1);
        LOG(FCGI,FCGI_PROC,"kind=[%d]",1);

        //title_m
        rop_hash_get(conn,FILEID_NAME_HASH,file_id_array[i],name);
        cJSON_AddStringToObject(item,"title_m",name);
        LOG(FCGI,FCGI_PROC,"title_m=[%s]",name);

        //title_s
        rop_hash_get(conn,FILEID_USER_HASH,file_id_array[i],user);
        LOG(FCGI,FCGI_PROC,"title_s=[%s]",user);
        cJSON_AddStringToObject(item,"title_s",user);

        //decrip

        rop_hash_get(conn,FILEID_TIME_HASH,file_id_array[i],time);
        cJSON_AddStringToObject(item,"descrip",time);
        LOG(FCGI,FCGI_PROC,"descrip=[%s]",time);

        //文件数据类型
        //picurl_m
        rop_hash_get(conn,FILEID_TYPE_HASH,file_id_array[i],type);
        strcat(pic_url,"http://");
        strcat(pic_url,STORAGE_IP);
        strcat(pic_url,"/static/file_png/");
        strcat(pic_url,type);
        strcat(pic_url,".png");

        cJSON_AddStringToObject(item,"picurl_m",pic_url);
        LOG(FCGI,FCGI_PROC,"picurl_m=[%s]",pic_url);
        //url 文件的下载地址
        rop_hash_get(conn,FILEID_URL_HASH,file_id_array[i],url);

        cJSON_AddStringToObject(item,"url",url);
        LOG(FCGI,FCGI_PROC,"url=[%s]",url);

        //pv

        rop_hash_get(conn,FILEID_PV_HASH,file_id_array[i],pv);
        cJSON_AddNumberToObject(item,"pv",atoi(pv));
        LOG(FCGI,FCGI_PROC,"pv=[%d]",atoi(pv));

        //文件共享状态hot
        rop_hash_get(conn,FILEID_SHARED_STATUS_HASH,file_id_array[i],share_status);
        printf("hot=%d\n",atoi(share_status)); 
        cJSON_AddNumberToObject(item,"hot",atoi(share_status));

        //将封装好的object添加到array中
        cJSON_AddItemToArray(array,item);

    }

    //将已经封装好的数组加入root中
    cJSON_AddItemToObject(root,"games",array);

    //将root变成json字符差
    p=cJSON_Print(root);

    //将p传出
    *out=p;
END:
    if(file_id_array!=NULL)
    {
        free(file_id_array);
    }
    
    if(conn!=NULL)
    {
        rop_disconnect(conn);
    }

    return retn;

}
