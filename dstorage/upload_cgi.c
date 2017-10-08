

#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/wait.h>
#include"upload_file.h"
#include "fcgi_stdio.h"
#include"fcgi_config.h"
#include"util.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"fdfs_api.h"
#include"make_log.h"
extern char**environ;
#define FCGI "fcgiupload_cgi"
#define FCGI_UPLOAD "data"


int get_username(char*user)
{
    char*query_string=getenv("QUERY_STRING");

    query_parse_key_value(query_string,"user",user,NULL);
    if(strlen(user)==0)
    {
        LOG(FCGI,FCGI_UPLOAD,"get user error");
        return -1;
    }
    LOG(FCGI,FCGI_UPLOAD,"user =%s",user);
    return 0;

}
int main ()
{
    int ret=0;
    // char **initialEnv = environ;
    //   int count = 0;
    char boundary[256]={0};
    char content[256]={0};
    char filename[256]={0};
    char user[256]={0};//当前文件所属用户名

    char file_id[FILE_ID_LEN]={0};
    while (FCGI_Accept() >= 0) {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n");
        //得到用户名
        //如果没有用户名直接提示用户登录
#if 1
        if(get_username(user))
        {
            LOG(FCGI,FCGI_UPLOAD,"上传文件失败，请先登录.\n");
            printf("上传文件失败，请先登录");
//            goto END;
            continue;
        }
#endif

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        }
        else {
            int i, ch;
            char*begin=NULL;

#if 0
            FILE *fp=NULL;
            char*buf=NULL;
            buf=(char*)malloc(sizeof(char)*len);
            memset(buf,0,len);
            char*p=NULL;
            p=buf;
            fp=fopen("./post_data","w");

            printf("Standard input:<br>\n<pre>\n");
#endif
            char *file_buf=NULL;
            char*p=NULL;
            char *q=NULL;
            char *k=NULL;
            file_buf=malloc(len);
            memset(file_buf,0,len);
            p=file_buf;
            for (i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //putchar(ch);
                *p=ch;
                p++;
            }
            printf("\n</pre><p>\n");
            FILE *post_fp=fopen("./post_data","w");
            fwrite(file_buf,1,len,post_fp);
            fclose(post_fp);
            //得到分割线
            begin=file_buf;
            p=strstr(begin,"\r\n");
            if(p==NULL)
            {
                printf("p is null.\n");
                goto END;
            }
            strncpy(boundary,begin,p-begin);

            printf("boundary:%s\n",boundary);
            LOG(FCGI,FCGI_UPLOAD,"boundary:%s\n",boundary);

            p+=2;
            len-=(p-begin);
            begin=p;

            p=strstr(begin,"\r\n");
            if(p==NULL)
            {
                goto END;
            }

            strncpy(content,begin,p-begin);
            q=strstr(begin,"filename=");
            q+=strlen("filename=");
            q++;
            k=strchr(q,'"');
            strncpy(filename,q,k-q);
            printf("filename=[%s]",filename);
            LOG(FCGI,FCGI_UPLOAD,"filename:%s\n",filename);

            p+=2;
            len-=(p-begin);

            begin=p;
            p=strstr(begin,"\r\n");
            p+=2;
            p+=2;//第五行
            len-=(p-begin);
            begin=p;

            printf("before memstr\n");
            p=memstr(begin,len,boundary);
            printf("after memstr\n");
            if(p==NULL)
            {
                goto END;
            }else{
                p=p-2;
            }

            int fd=0;
            fd=open(filename,O_CREAT|O_WRONLY,0644);

            if(fd<0)
            {
                printf("open %s error",filename);
                LOG(FCGI,FCGI_UPLOAD,"open %s error\n",filename);
                goto END;
            }

            ftruncate(fd,(p-begin));
            write(fd,begin,(p-begin));
            close(fd);
            printf("上传文件成功");

            //3将filename 上传到fastDFSD里
#if 0
            ret=fdfs_upload_by_filename(filename,file_buf);
            if(ret!=0)
            {
                printf("upload fastdfs error\n");
                goto END;
            }
#endif
            //4将filename的一些关系入库redis
            LOG(FCGI,FCGI_UPLOAD,"before save_one_file filename=%s\n",filename);
            ret=save_one_file(filename,user);
            if(ret!=0)
            {
                printf("%s storage erro",filename);
                goto END;
            }

            //5将临时文件删除
            unlink(filename);
END:
            LOG(FCGI,FCGI_UPLOAD,"END is working.\n");
        memset(content,0,256);
        memset(boundary,0,256);
        memset(filename,0,256);
        memset(file_id,0,FILE_ID_LEN);
        if(file_buf!=NULL)
        {
            free(file_buf);
        }
        }

    } /* while */

    return 0;
}
