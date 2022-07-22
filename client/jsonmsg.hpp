#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <jsoncpp/json/json.h>
#include "protocol.hpp"
using namespace std;

//第一次客户端发送的数据进行序列化
string FirRequsetSerialize(const FirstRequset &req)
{
    Json::Value root;
    root["logstatus"] = req.logstatus; //登录状态
    root["type"] = req.type;
    root["nickname"] = req.nickname;
    root["password"] = req.password;
    root["fdfrom"] = req.fdfrom;//发送者的fd
    root["fdto"] = req.fdto;//接收者的fd
    root["ifonline"] = req.ifonline;//是否在线
    root["tonickname"]=req.tonickname;
    root["message"]=req.message;
    root["groupname"]=req.groupname;
    Json::FastWriter writer;
    string sendwriter = writer.write(root);
    return sendwriter;
}

//第一次服务器接收到数据进行反序列化
void FirRequsetReSerialize(const string &jsonstring, FirstRequset &req)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring, root);
    req.type = root["type"].asInt();
    req.nickname = root["nickname"].asString();
    req.password = root["password"].asString();
    req.type=root["type"].asInt();
    req.fdfrom=root["fdfrom"].asInt();
    req.fdto=root["fdto"].asInt();
    req.ifonline=root["ifonline"].asBool();//查看是否在线
    req.message=root["message"].asString();
    req.logstatus=root["logstatus"].asInt();
    req.tonickname=root["tonickname"].asString();
    req.groupname=root["groupname"].asString();

}

//对服务器第一次响应进行序列化
string FirstResponseSerialize(const FirstResponse &res)
{
    Json::Value root;
    root["status"] = res.status;
    root["msg"] = res.msg;
    Json::FastWriter writer;
    string sendwriter = writer.write(root);
    return sendwriter;
}

//对服务器接收到服务器的响应进行反序列化
void FirResponseReSerialize(const string &jsonstring, FirstResponse &res)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring, root);
    res.status = root["status"].asInt();
    res.msg = root["msg"].asString();
}

