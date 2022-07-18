#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
#include <hiredis/hiredis.h>
class Myredis
{
private:
    redisContext *c;
    redisReply *pm_rr;

public:
    Myredis()
    {
    }
    ~Myredis()
    {
    }
    void connect() //连接
    {
        c = redisConnect((char *)"127.0.0.1", 6379);
        if (c == NULL || c->err)
        {
            if (c->err)
                printf("Error:%s\n", c->errstr);
            else
                printf("cant allocate redis context\n");
        }
    }
    void disconnect() //断开连接
    {
        redisFree(c);
        freeReplyObject(pm_rr);
    }
   
    void DoCommand(string command)
    {
        pm_rr=(redisReply*)redisCommand(c,command.c_str());
    }

    bool isExist(string command)//判断某个数据是否在这个数据库里面
    {
        pm_rr=(redisReply*)redisCommand(c,command.c_str());
        return pm_rr->integer;//
    }
    string GetData(string command)//获得数据库里面的数据
    {
        pm_rr=(redisReply*)redisCommand(c,command.c_str());
        return pm_rr->str;
    }
    void DelData(string command)//删除数据库里面的数据
    {
        pm_rr=(redisReply*)redisCommand(c,command.c_str());
    }
};