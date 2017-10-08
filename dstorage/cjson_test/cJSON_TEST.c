#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include"cJSON.h"


void test1()
{
    cJSON*root=NULL;
    char*json_str=NULL;

    json_str=malloc(4096);
    memset(json_str,0,4096);

    FILE *fp=NULL;
    fp=fopen("./json_test.json","r");
    fread(json_str,4096,1,fp);
    fclose(fp);
    
    printf("json_str=[%s]\n",json_str);
    root=cJSON_Parse(json_str);

    cJSON*name_obj=cJSON_GetObjectItem(root,"name");
    printf("name=%s\n",name_obj->valuestring);

    cJSON*age_obj=cJSON_GetObjectItem(root,"age");
    printf("age=%d\n",age_obj->valueint);

    cJSON*array_obj=cJSON_GetObjectItem(root,"array");
    
    int array_len=cJSON_GetArraySize(array_obj);
    int i=0;
    for(i=0;i<array_len;i++)
    {
        cJSON* item=cJSON_GetArrayItem(array_obj,i);
        printf("array[%d]=%s\n",i,item->valuestring);
    }


    if(root!=NULL)
    {
        cJSON_Delete(root);
    }


}

//cjson->str
void test2()
{
    cJSON*root=NULL;
    cJSON*array_obj=NULL;
    int i=0;

    root=cJSON_CreateObject();
    
    array_obj=cJSON_CreateArray();

    for(i=0;i<3;i++)
    {
        cJSON*item=cJSON_CreateObject();
        cJSON_AddStringToObject(item,"id","group1/M00/00/12321321.jpg");
        cJSON_AddNumberToObject(item,"kind",1);

        cJSON_AddItemToArray(array_obj,item);

    }

    //将已经封装号的数组添加到{}root中
    cJSON_AddItemToObject(root,"games",array_obj);

    char*json_str=cJSON_Print(root);

    printf("json_str=[%s]\n",json_str);

    if(root!=NULL)
    {
        cJSON_Delete(root);
    }

    if(json_str!=NULL)
    {
        free(json_str);
    }

}
int main(int argc,char*argv[])
{
//    test1();
    test2();
    return 0;
}
