#pragma once
#include <iostream>
#include "protocol.hpp"

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


void FriendCheckMember(int sockfd,FirstRequset&req)
{
    string msg=FirRequsetSerialize(req);
    send(sockfd,msg.c_str(),msg.size(),0);

}
void FriendAdd(int sockfd,FirstRequset&req)
{
    //添加好友
    cout<<"请输入您要添加的用户: ";
    cin>>req.tonickname;
    string msg=FirRequsetSerialize(req);
    send(sockfd,msg.c_str(),msg.size(),0);
    
    FirstResponse recvres;
    char buf[MAX_SIZE];
    memset(buf, 0, sizeof(buf));
    string tmp = RecvMsg(sockfd, buf, sizeof(buf) - 1);

    FirResponseReSerialize(tmp, recvres); //进行反序列化
    if (recvres.status == SUCCESS)
    {
        cout << recvres.msg << endl;
    }
    else if (recvres.status == Failure)
    {
        cout << recvres.msg << endl;
    }
}

void LoginChatRoom(int sockfd,FirstRequset&sreq)//这个里面就有之前我们输入的名字和密码
{
    sreq.ifonline=true;
    sreq.logstatus=LOGINAFTER;//设置为登录后的状态
    do
    {
        loadmenu();
        cin >> sreq.type;
        switch (sreq.type)
        {
        case FRIEND_CHECK_MEMBER:
            FriendCheckMember(sockfd,sreq);
            break;
        case FRIEND_ADD:
            FriendAdd(sockfd,sreq);
            break;
        case FRIEND_DEL:
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
    } while (sreq.type != -1);
}