#pragma once

#define MAX_SIZE     1024

#define LOGIN        1
#define REGISTER     2
#define LOGOUT       3
#define QUIT         4


#define SUCCESS      0
#define Failure      -1

#include<string>
#include<iostream>
using namespace std;
struct FirstRequset//第一次给服务器发送的消息
{
    int type;//对服务器的4种类型
    string nickname;//如果有注册请求,使用的名字
    string password;//密码
    FirstRequset()=default;//生成默认构造函数
};

struct FirstResponse//服务器第一次发送
{
    int status;//0表示成功，-1表示失败
    string msg;//服务器给客户端发送的响应,成功与否
    FirstResponse()=default;
};