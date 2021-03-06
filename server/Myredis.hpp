#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
#include <hiredis/hiredis.h>
#include <vector>
#include<map>
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

    void AddData(string command) //增加数据
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
    }

    bool isExist(string command) //判断某个数据是否在这个数据库里面
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        return pm_rr->integer; //
    }
    string GetAData(string command) //获得数据库里面的数据
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        return pm_rr->str;
    }
    string GetVectorString(string command) //里面是一个数组存储字符串
    {
        string ret;
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        for (int i = 0; i < pm_rr->elements; i++)
        {
            ret += pm_rr->element[i]->str; //获得str里面的每一个值
            ret += " ";
        }
        return ret;
    }
    void DelData(string command) //删除数据库里面的数据
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
    }
    vector<string> GetHashData(string command) //在hash表里面获得字符串
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        string s = pm_rr->str;
        vector<string> ret;
        int j = 0;
        string buf = "";
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == '|')
            {
                string l = buf;
                ret.push_back(l); //尾插上这个字符串
                buf = "";
            }
            else
            {
                buf.push_back(s[i]);
            }
        }
        ret.push_back(buf);
        return ret;
    }
    vector<string> GetListData(string command)//获得list里面的所有值
    {
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        vector<string> ret;
        for (int i = 0; i < pm_rr->elements; i++)
        {
            ret.push_back(pm_rr->element[i]->str);
        }
        return ret;
    }
    void SetData(string command)
    {
        //设置数据库里面的数据
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
    }
    map<string, string> GetGroupApplyInfo(string command) //获得申请加入群的名单，包含群+人
    {
        map<string, string> ret;
        string key, value;
        pm_rr = (redisReply *)redisCommand(c, command.c_str());
        for (int i = 0; i < pm_rr->elements; i++)
        {
            string ll = pm_rr->element[i]->str;
            size_t pos = ll.find("|");
            value = ll.substr(0, pos);
            key = ll.substr(pos + 1);
            ret[key] = value;
        }
        return ret;
    }
};