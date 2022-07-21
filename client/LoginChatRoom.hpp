#pragma once
#include <iostream>
#include "protocol.hpp"
#include "IO.hpp"

void loadmenu(FirstRequset& req)
{
    cout << "---------------------------------------" << endl;
    cout << "     Enjoy      ***"<<req.nickname<<"***     Yourself       " << endl;
    cout << "---------------------------------------" << endl;
    cout << "        Start Use The ChatRoom         " << endl;
    cout << "          [-2]查看未读消息              " << endl;
    cout << "   [1]查看好友        [2]添加好友       " << endl;
    cout << "   [3]删除好友        [4]与好友进行聊天 " << endl;
    cout << "         [0]查看好友在线情况            " << endl;
    cout << "   [5]创建群          [6]加入群         " << endl;
    cout << "   [7]退出群          [9]查看所有群     " << endl;
    cout << "          [10]对群进行处理               " << endl;
    cout << "          [-1]退出登录                   " << endl;
    cout << "---------------------------------------" << endl;
    cout << "请进行您的选择: ";
}

void FriendCheckMember(int sockfd, FirstRequset &req) //查看好友列表
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    cout << "您的好友列表如下" << endl;
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}
void FriendAdd(int sockfd, FirstRequset &req)
{
    //添加好友
    cout << "请输入您要添加的好友: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);

    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}
void FriendDel(int sockfd, FirstRequset &req) //删除好友
{
    cout << "请输入您要删除的好友: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(10);
    system("clear");
}

void LOADEXIT(int sockfd, FirstRequset &req)
{
    //给对对方发送退出登录的请求
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd); //接收到退出成功的消息
    sleep(2);
    system("clear");
}

void FriendCHECKONLINE(int sockfd, FirstRequset &req) //查看好友是否在线
{
    //先要放在那边听
    cout << "请输入您要查询的好友在线情况: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void *Read(void *args)
{
    pthread_detach(pthread_self());
    FirstRequset req = *(FirstRequset *)args;

    while (1)
    {
        FirstResponse resp = RecvReSerializeMsgForLogin(req.fdfrom);
        if (resp.msg.find("exit") != string::npos)
        {
            break;
        }
    }
}

void FriendCHAT(int sockfd, FirstRequset &req) //聊天
{
    pthread_t reader;
    req.fdfrom = sockfd;
    pthread_create(&reader, nullptr, Read, (void *)&req); //一进来就要去接收
    cout << "请输入您要进行聊天的好友: ";

    cin >> req.tonickname;
    while (1)
    {
        // cout << "请发送您要发送的消息: ";
        cin >> req.message; //发送的消息
        string msg = FirRequsetSerialize(req);
        send(sockfd, msg.c_str(), msg.size(), 0);
        if (req.message == "exit") //发送这个就退出了
        {
            //退出
            break;
        }
    }
}

void CheckUnReadMsg(int sockfd, FirstRequset &req)
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    // system("clear");
}

void LoginChatRoom(int sockfd, FirstRequset &sreq) //这个里面就有之前我们输入的名字和密码
{
    sreq.ifonline = true;
    sreq.logstatus = LOGINAFTER; //设置为登录后的状态
    do
    {
        loadmenu(sreq);
        cin >> sreq.type;
        switch (sreq.type)
        {
        case CHECKUNREADMESSAGE:
            CheckUnReadMsg(sockfd, sreq);
            break;
        case FRIEND_CHECK_MEMBER:
            FriendCheckMember(sockfd, sreq); //查看好友有多少
            break;
        case FRIEND_ADD:
            FriendAdd(sockfd, sreq); //添加好友
            break;
        case FRIEND_DEL:
            FriendDel(sockfd, sreq); //删除好友
            break;
        case FRIEND_CHAT:
            FriendCHAT(sockfd, sreq); //与好友进行聊天
            break;
        case FRIEND_CHECK_ONLINE:
            FriendCHECKONLINE(sockfd, sreq); //查看好友是否在线
            break;
        case GROUP_CREATE:
            break;
        case GROUP_ADD:
            break;
        case GROUP_QUIT:
            break;
        case GROUP_CHECK:
            break;
        case GROUP_MANAGE:
            break;
        case LEFTLOAD:
            LOADEXIT(sockfd, sreq);
            break;
        }
    } while (sreq.type != -1); //-1就退出登录了
}