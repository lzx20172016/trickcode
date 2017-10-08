#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"fcgi_stdio.h"


int main(int argc,char*argv[])
{
    int count=0;

    while(FCGI_Accept()>=0)
    {
        printf("Content-type:text/html\r\n");
        printf("\r\n");
        printf("<title>Fast CGI Hello!</title>");
        printf("<h1>Fast CGI Hello!</h1>");
        printf("Request number %d running on host <i>%s</i>\n",++count,getenv("SERVER_NAME"));
        printf("</br>client ip=%s,client port=%s\n",getenv("REMOTE_ADDR"),getenv("REMOTE_PORT"));
        printf("</br>server ip=%s,server port=%s\n",getenv("SERVER_ADDR"),getenv("SERVER_PORT"));
        printf("</br>query_string=%s",getenv("QUERY_STRING"));
    }
    return 0;
}
