#ifndef __UPLOAD_FILE_H__
#define __UPLOAD_FILE_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

#include"fdfs_api.h"
#include"redis_op.h"
#include"redis_keys.h"
#include"../make_log.h"
#define TIME_STR_MAX (256)
#define FILE_URL_LEN (256)
#define STORAGE_IP ("192.168.0.118")

int save_one_file(char*filename);
int select_files(int fromId,int count,char**out);

#endif
