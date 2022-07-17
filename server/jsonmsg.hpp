#pragma once 
#include <iostream>
#include <string>
#include <pthread.h>
#include <jsoncpp/json/json.h>
#include"protocol.hpp"
using namespace std;


//第一次客户端发送的数据进行序列化
string FirRequsetSerialize(const FirstRequset &req)
{
    Json::Value root;
    root["type"]=req.type;
    root["nickname"]=req.nickname;
    root["password"]=req.password;
    Json::FastWriter writer;
    string sendwriter=writer.write(root);
    return sendwriter;
}

//第一次服务器接收到数据进行反序列化
void FirRequsetReSerialize(const string &jsonstring,FirstRequset& req)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring,root);
    req.type=root["type"].asInt();
    req.nickname=root["nickname"].asString();
    req.password=root["password"].asString();
}

//对服务器第一次响应进行序列化
string FirstResponseSerialize(const FirstResponse&res)
{
    Json::Value root;
    root["status"]=res.status;
    root["msg"]=res.msg;
    Json::FastWriter writer;
    string sendwriter=writer.write(root);
    return sendwriter;
}


//对服务器接收到服务器的响应进行反序列化
void FirResponseReSerialize(const string &jsonstring,FirstResponse& res)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring,root);
    res.status=root["status"].asInt();
    res.msg=root["msg"].asString();
}

