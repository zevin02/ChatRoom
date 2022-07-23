#pragma once

#define MAX_SIZE     1024

#define LOGIN        1
#define REGISTER     2
#define LOGOUT       3
#define QUIT         4


#define SUCCESS      0
#define Failure      -1




#define CHECKUNREADMESSAGE   -2
#define FRIEND_CHECK_MEMBER  1//查看是否在线
#define FRIEND_ADD    2      
#define FRIEND_DEL    3   
#define FRIEND_CHAT_ONLINE   4
#define FRIEND_CHAT_UNONLINE   8    //和不在线的好友进行聊天
#define FRIEND_CHECK_ONLINE       0
#define FRIEND_CHAT_HISTORY     11  //查看历史聊天记录



#define LEFTLOAD     -1


//对群进行整体的处理

#define GROUP_CREATE  5    //创建一个群
#define GROUP_ADD     6    //申请加入一个群
#define GROUP_QUIT    7    //退出一个群
#define GROUP_CHECK   9    //查看加入了哪些群


//对某一个群进行操作，在group_manage里面
#define GROUP_MANAGE_DELMEMBER        11//删除一个群成员
#define GROUP_MANAGE_ADDMANAGER       12//添加一个管理员
#define GROUP_MANAGE_DESTROY          13//作为群主解散一个群
#define GROUP_MANAGE_CHAT             14//进行群聊天
#define GROUP_MANAGE_DELMANAGER       15//删除群管理
#define GROUP_MANAGE_VIEWMEMBERLIST   16//查看群成员列表
#define GROUP_DELMEMBER               17//群主和群管理把群成员给删除掉
#define GROUP_APPLY                   18//申请加入一个群
#define GROUP_MEMBERADDEXECUTE        19//群主查看完消息之后执行加入成员
#define GROUP_CHECKAPPLY              20//查看群申请消息 



//用来区分发送的协议
#define LOGINAFTER    0
#define LOGINBEFORE   1








#include<string>
#include<iostream>
#include<queue>
using namespace std;
struct FirstRequset//第一次给服务器发送的消息
{
    int logstatus;//登录状态
    int type;//对服务器的4种类型
    string nickname;//如果有注册请求,使用的名字
    string password;//密码
    bool ifonline;//判断是否在线
    int fdfrom;//来自谁
    int fdto;//发给谁
    string tonickname;//要操作的昵称
    string groupname;//要操作的群名
    string message;//要发送的消息
    FirstRequset()=default;//生成默认构造函数
};



struct FirstResponse//服务器第一次发送
{
    int status;//0表示成功，-1表示失败
    string msg;//服务器给客户端发送的响应,成功与否
    FirstResponse()=default;
};






