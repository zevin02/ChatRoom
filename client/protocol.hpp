#pragma once

#define MAX_SIZE     1024

#define LOGIN        1
#define REGISTER     2
#define LOGOUT       3
#define QUIT         4


#define SUCCESS      0
#define Failure      -1


#define FRIEND_CHECK_MEMBER  1//查看是否在线
#define FRIEND_ADD    2      
#define FRIEND_DEK    3   
#define FRIEND_CHAT   4
#define FRIEND_CHECK_ONLINE       0



//对群进行整体的处理

#define GROUP_CREATE  5    //创建一个群
#define GROUP_ADD     6    //申请加入一个群
#define GROUP_QUIT    7    //退出一个群
#define GROUP_CHECK   9    //查看加入了哪些群
#define GROUP_MANAGE  10   //后续处理群的各种操作


//对某一个群进行操作，在group_manage里面
#define GROUP_MANAGE_DELMEMBER    11//删除一个群成员
#define GROUP_MANAGE_ADDMANAGER   12//添加一个管理员
#define GROUP_MANAGE_DEL          13//作为群主解散一个群
#define GROUP_MANAGE_CHAT         14//进行群聊天
#define GROUP_MANAGE_DELMANAGER   15//删除群管理
#define GROUP_MANAGE_VIEWMEMBERLIST   16//查看群成员列表












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



struct SecondRequset
{
    int type;//执行上面操作
    int fdfrom;//发送者的套接字
    int fdto;//接收者的fd
    bool ifonline;//查看是否在线
};
