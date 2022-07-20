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
    pthread_mutex_t lock;
    pthread_mutex_init(&lock,nullptr);

    FirstRequset req = *(FirstRequset *)args;

    pthread_mutex_lock(&lock);
    while(1)
    RecvReSerializeMsg(req.fdfrom);
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
}
void* Write(void* args)
{
    pthread_detach(pthread_self());
    pthread_mutex_t lock;
    pthread_mutex_init(&lock,nullptr);
    pthread_detach(pthread_self());
    FirstRequset req = *(FirstRequset *)args;

    pthread_mutex_lock(&lock);
    while(1)
    {
        cin>>req.message;//发送的消息
        send(req.fdfrom,req.message.c_str(),req.message.size(),0);
    }
    //发送消息
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);


}

void FriendCHAT(int sockfd, FirstRequset &req) //查看好友是否在线
{
    pthread_t reader,writer;
    req.fdfrom=sockfd;
    pthread_create(&reader,nullptr,Read,(void*)&req);
    // thread reader(Read,sockfd);//这个线程接收消息
    cout << "请输入您要进行聊天的好友: ";

    cin >> req.tonickname;
    pthread_create(&writer,nullptr,Write,(void*)&req);//
    string msg = FirRequsetSerialize(req);
    send(sockfd, msg.c_str(), msg.size(), 0);
    // thread writer(Write,sockfd,req);//这个线程发送消息

    // RecvReSerializeMsg(sockfd);

    // reader.detach();//线程分离
    // writer.detach();
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