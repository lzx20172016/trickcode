#ifndef __REDIS_KEYS_H__
#define __REDIS_KEYS_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

#include"fdfs_api.h"
#include"redis_op.h"
#include"../make_log.h"
#define TIME_STR_MAX (256)
#define FILE_URL_LEN (256)
#define STORAGE_IP ("192.168.0.118")
#define STORAGE_PORT (80)
#define REDIS_IP "127.0.0.1"
#define REDIS_PORT "6379"
#define FILE_PUBLIC_LIST "FILE_PUBLIC_LIST"

#define USER_USERID_HASH "USER_USERID_HASH"
#define FILE_USER_LIST_ "FILE_USER_LIST_"
#define FILE_REFERENCE_COUNT_HASH "FILE_REFERENCE_COUNT_HASH"

#define FILE_USER_LIST "FILE_INFO_LIST"
#define FILEID_NAME_HASH "FILEID_NAME_HASH"
#define FILEID_TIME_HASH "FILEID_TIME_HASH"
#define FILEID_USER_HASH "FILEID_USER_HASH"
#define FILEID_PV_HASH "FILEID_PV_HASH"
#define FILEID_TYPE_HASH "FILEID_TYPE_HASH"
#define FILEID_URL_HASH "FILEID_URL_HASH"
#define FILEID_SHARED_STATUS_HASH "FILEID_SHARED_STATUS_HASH"


//int save_one_file(char*filename,char*user);
//int select_files(int fromId,int count,char**out);

#endif
