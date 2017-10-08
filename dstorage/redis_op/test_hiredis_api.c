#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<hiredis.h>

int main(int argc, char **argv) {
    unsigned int j;
    redisContext *c;
    redisReply *reply;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnect("127.0.0.1",6379);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */

    /* Set a key */
    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    if(reply==NULL)
    {
        printf("set error\n");
    }
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    reply=redisCommand(c,"Get %s","foo");
    if(reply==NULL)
    {
        printf("set error\n");
    }

    printf("value=%s,len=%lu\n",reply->str,reply->len);
    freeReplyObject(reply);



    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
