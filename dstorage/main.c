#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#include"fdfs_api.h"
int main(int argc,char*argv[])
{
    int ret;
    char *filename="/home/lzx/aa.cpp";
    char fileid[FILE_ID_LEN]={0};
    ret=fdfs_upload_by_filename(filename,fileid);
    if(ret!=0)
    {
printf("upload %s error\n",filename);
exit(1);
    }


    printf("fileid=%s\n",fileid);
    return 0;
}
