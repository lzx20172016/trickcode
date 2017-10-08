#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#include"redis_op.h"
#include"../make_log.h"
int main(int argc, char **argv) {
    redisContext*conn=NULL;
    int ret=0;
    char value[VALUES_ID_SIZE]={0};
    int i=0;
    
    RVALUES value_array=NULL;

    conn=rop_connectdb_nopwd("127.0.0.1","6379");
    if(conn==NULL)
    {
            printf("connect error");
            return 1;
    }

    ret=rop_set_string(conn,"FOO","nihaoshijie");
    if(ret!=0)
    {
        return 1;
    }
   
    ret=rop_get_string(conn,"FOO",value);
    if(ret!=0){
    
        return 1;
    }


    printf("value=%s\n",value);

    //创建链表
    ret=rop_list_push(conn,"my_list_key","zhang2");
    if(ret!=0){
    printf("after rop_list_push.\n");    
        return 1;
    }
    
    ret=rop_list_push(conn,"my_list_key","zhang3");
    if(ret!=0){
    
        return 1;
    }

    ret=rop_list_push(conn,"my_list_key","wangwu3");
    if(ret!=0){
    
        printf("after rop_list_push.\n");
        return 1;
    }

    int count=8;
    int array_len=0;
    value_array=malloc(count*VALUES_ID_SIZE);
    memset(value_array,0,count*VALUES_ID_SIZE);
    printf("before rop_range_list.\n");
    ret=rop_range_list(conn,"my_list_key",0,count-1,value_array,&array_len);
    if(ret!=0){
        printf("after rop_range_list.\n");
    
        return 1;
    }
    printf("after rop_range_list correct.\n");

    for(i=0;i<array_len;i++)
    {
        printf("value_array[%d]=%s\n",i,value_array[i]);
    }
    free(value_array);
    printf("-------------------------------------\n");
    ret=rop_hash_set(conn,"my_hash_key","name","zhang3");
    if(ret!=0){
        printf("hset error.\n");
    
        return 1;
    }
    ret=rop_hash_set(conn,"my_hash_key","age","18");
    if(ret!=0){
        printf("hset error.\n");
    
        return 1;
    }

    memset(value,0,VALUES_ID_SIZE);
    ret=rop_hash_get(conn,"my_hash_key","name",value);
    if(ret!=0){
        printf("hget error.\n");
    
        return 1;
    }
    printf("hget succ:value=%s\n",value);

    rop_disconnect(conn);
    return 0;
}
