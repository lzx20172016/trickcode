#ifndef __UTIL_H_
#define __UTIL_H_
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char*memstr(char*full_data,int full_data_len,char*substr);


int get_file_suffix(const char *file_name, char *suffix);

int query_parse_key_value(const char *query, const char *key, char *value, int *value_len_p);


void str_replace(char* strSrc, char* strFind, char* strReplace);
#endif
