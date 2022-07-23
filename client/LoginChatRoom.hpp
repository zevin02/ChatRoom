#pragma once
#include <iostream>
#include "protocol.hpp"
#include "IO.hpp"

void loadmenu(FirstRequset &req)
{

    cout << " ____ _   _    _  _____ ____   ___   ___  __  __ " << endl;
    cout << "/ ___| | | |  / \\|_   _|  _ \\ / _ \\ / _ \\|  \\/  |" << endl;
    cout << "| |   | |_| | / _ \\ | | | |_) | | | | | | | |\\/| |" << endl;
    cout << "| |___|  _  |/ ___ \\| | |  _ <| |_| | |_| | |  | |" << endl;
    cout << " \\____|_| |_/_/   \\_\\_| |_| \\_\\___/ \\___/|_|  |_|" << endl;

    cout << "----------------------------------------------------------" << endl;
    cout << "     Enjoy      ***" << req.nickname << "***     Yourself       " << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "             Start Use The ChatRoom         " << endl;
    cout << "   [1]查看好友                  [2]添加好友       " << endl;
    cout << "   [3]删除好友                  [-2]查看未读消息" << endl;
    cout << "  [**您在聊天的时候请先查看对方是否在线，再选择下列的操作**]" << endl;
    cout << "     [4]与在线好友进行聊天       [8]与不在线好友聊天" << endl;
    cout << "     [0]查看好友在线情况      [11]查看与好友的历史消息        " << endl;
    cout << "   [5]创建群                    [6]加入群(强制加入)          " << endl;
    cout << "   [7]退出群                    [9]查看所有群                " << endl;
    cout << "   [10]对群进行处理             [16]查看群成员信息            " << endl;
    cout << "   [12]设置群管理               [-1]退出登录        " << endl;
    cout << "   [15]删除群管理               [17]删除群成员      " << endl;
    cout << "     [13]销毁一个群             [18]申请加入群      " << endl;
    cout << "     [19]添加群用户             [20]查看群申请消息 " << endl;
    cout << "--------------------------------------------------------" << endl;
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

void FriendCHATONLINE(int sockfd, FirstRequset &req) //在线聊天
{
    //聊天之前我们要先知道用问在线与否，在线就聊天
    //这里先发送获取它是否在线
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

void FriendCHATUNONLINE(int sockfd, FirstRequset &req) //不在线聊天
{
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

void CheckUnReadMsg(int sockfd, FirstRequset &req) //查看未读消息
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(10);
}

void FriendCHATHISTORY(int sockfd,FirstRequset& req)//查看历史聊天记录
{
    cout<<"请选择你要查看历史聊天记录的好友: ";
    cin>>req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(10);
}
void GroupCREATE(int sockfd, FirstRequset &req) //创建群
{
    cout << "请输入您要创建的群的名称： ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupCHECK(int sockfd, FirstRequset &req) //查看我所有加入的群
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
}

void GroupCHECKMEMBERLIST(int sockfd, FirstRequset &req) //查看群成员列表
{
    cout << "请输入您要查看群成员的群的名称： ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    cout << req.groupname << "群的成员列表如下" << endl;
    RecvReSerializeMsg(sockfd);
    sleep(5);
    system("clear");
}

void GroupQUIT(int sockfd, FirstRequset &req) //退出一个群
{
    cout << "请输入你要退出的群名称： ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupADDMANAGER(int sockfd, FirstRequset &req) //添加群管理员
{
    cout << "请输入您想要操作的群: ";
    cin >> req.groupname;
    cout << "请输入您想要添加为管理员的成员名: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupADD(int sockfd, FirstRequset &req) //添加一个群
{
    cout << "请输入您要加入的群: ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}
void GroupDELMANAGER(int sockfd, FirstRequset &req) //删除群管理员
{
    cout << "请输入您想要操作的群: ";
    cin >> req.groupname;
    cout << "请输入您想要删除的管理员的成员名: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupDELMEMBER(int sockfd, FirstRequset &req) //群主和群管理把群成员删除掉
{
    //这里我们默认，删除的对象只是一个普通群成员
    cout << "请输入您想要操作的群: ";
    cin >> req.groupname;
    cout << "请输入您想要删除的群成员的成员名: ";
    cin >> req.tonickname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupDESTROY(int sockfd, FirstRequset &req) //销毁一个群
{
    cout << "请输入您要解散的群: ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupAPPLY(int sockfd, FirstRequset &req) //申请加入一个群
{
    cout << "请输入您要申请加入的群: ";
    cin >> req.groupname;
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
    system("clear");
}

void GroupCHECKAPPLY(int sockfd, FirstRequset &req) //作为群主去查看申请列表
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    cout << "申请加入您的群的人员如下" << endl;
    RecvReSerializeMsg(sockfd);
    sleep(10);
}

void GroupMEMBERADDEXECUTE(int sockfd, FirstRequset &req) //群主去执行添加好友的操作,对全部的添加都同意
{
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    RecvReSerializeMsg(sockfd);
    sleep(2);
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
            FriendCHATONLINE(sockfd, sreq); //与好友进行聊天
            break;
        case FRIEND_CHECK_ONLINE:
            FriendCHECKONLINE(sockfd, sreq); //查看好友是否在线
            break;
        case FRIEND_CHAT_UNONLINE: //给不在线的用户用的
            FriendCHATUNONLINE(sockfd, sreq);
            break;
        case FRIEND_CHAT_HISTORY: //查看历史聊天记录
            FriendCHATHISTORY(sockfd, sreq);
            break;
        case GROUP_CREATE: //创建群
            GroupCREATE(sockfd, sreq);
            break;
        case GROUP_ADD: //加入一个群
            GroupADD(sockfd, sreq);
            break;
        case GROUP_QUIT: //退出一个群
            GroupQUIT(sockfd, sreq);
            break;
        case GROUP_CHECK: //查看已经加入的群
            GroupCHECK(sockfd, sreq);
            break;
        case GROUP_MANAGE_VIEWMEMBERLIST: //查看群成员列表
            GroupCHECKMEMBERLIST(sockfd, sreq);
            break;
        case GROUP_MANAGE_ADDMANAGER: //添加一个群管理
            GroupADDMANAGER(sockfd, sreq);
            break;
        case GROUP_MANAGE_DELMANAGER: //删除一个群管理
            GroupDELMANAGER(sockfd, sreq);
            break;
        case GROUP_MANAGE_DESTROY: //解散一个群
            GroupDESTROY(sockfd, sreq);
            break;
        case GROUP_DELMEMBER: //删除一个群成员
            GroupDELMEMBER(sockfd, sreq);
            break;
        case GROUP_APPLY: //申请加入群
            GroupAPPLY(sockfd, sreq);
            break;
        case GROUP_CHECKAPPLY: //查看申请加入群的人
            GroupCHECKAPPLY(sockfd, sreq);
            break;
        case GROUP_MEMBERADDEXECUTE: //作为群主去执行把申请人加入群的操作
            GroupMEMBERADDEXECUTE(sockfd, sreq);
            break;
        case LEFTLOAD: //退出登录
            LOADEXIT(sockfd, sreq);
            break;
        }
    } while (sreq.type != -1); //-1就退出登录了
}