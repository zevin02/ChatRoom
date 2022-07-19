#pragma once
#include <iostream>
#include "protocol.hpp"
#include "IO.hpp"
void loadmenu()
{
    cout << "---------------------------------------" << endl;
    cout << "     Enjoy              Yourself       " << endl;
    cout << "---------------------------------------" << endl;
    cout << "        Start Use The ChatRoom         " << endl;
    cout << "   [1]查看好友        [2]添加好友       " << endl;
    cout << "   [3]删除好友        [4]与好友进行聊天 " << endl;
    cout << "         [0]查看好友在线情况            " << endl;
    cout << "   [5]创建群          [6]加入群         " << endl;
    cout << "   [7]退出群          [9]查看所有群     " << endl;
    cout << "         [10]对群进行处理               " << endl;
    cout << "         [-1]退出登录                   " << endl;
    cout << "---------------------------------------" << endl;
    cout << "请进行您的选择: ";
}

void FriendCheckMember(int sockfd, FirstRequset &req) //查看好友列表
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    cout<<"您的好友列表如下"<<endl;
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
void LoginChatRoom(int sockfd, FirstRequset &sreq) //这个里面就有之前我们输入的名字和密码
{
    sreq.ifonline = true;
    sreq.logstatus = LOGINAFTER; //设置为登录后的状态
    do
    {
        loadmenu();
        cin >> sreq.type;
        switch (sreq.type)
        {
        case FRIEND_CHECK_MEMBER:
            FriendCheckMember(sockfd, sreq);
            break;
        case FRIEND_ADD:
            FriendAdd(sockfd, sreq);
            break;
        case FRIEND_DEL:
            FriendDel(sockfd, sreq); //删除好友
            break;
        case FRIEND_CHAT:
            break;
        case FRIEND_CHECK_ONLINE:
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
        }
    } while (sreq.type != -1);//-1就退出登录了
}