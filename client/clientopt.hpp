#pragma once
#include <cstdlib>
#include "protocol.hpp"
#include "jsonmsg.hpp"
#include "LoginChatRoom.hpp"
void ClientLogin(FirstRequset &msg, int sockfd) //登录
{
    cout << "------------------------------------" << endl;
    cout << "         请开始您的登录" << endl;
    cout << "------------------------------------" << endl;
    cout << "请输入您的昵称 :";
    cin >> msg.nickname;
    cout << "请输入您的密码 :";
    cin >> msg.password;
    msg.logstatus = LOGINBEFORE;

    string req = FirRequsetSerialize(msg);
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
                                              // FirstResponse recvmsg;
    //接收服务端发送的消息进行
    system("clear");
    FirstResponse resp= RecvReSerializeMsgForLogin(sockfd);
    if(resp.status==SUCCESS)
    {
        LoginChatRoom(sockfd,msg);
    }
}
void ClientRegister(FirstRequset &msg, int sockfd) //注册
{
    cout << "------------------------------------" << endl;
    cout << "        请开始注册您的信息            " << endl;
    cout << "------------------------------------" << endl;
    cout << "请输入您要注册的昵称 :";
    cin >> msg.nickname;
    cout << "请输入您的密码 :";
    cin >> msg.password;

    msg.logstatus = LOGINBEFORE;
    //进行序列化，把消息发送给服务器
    string req = FirRequsetSerialize(msg);

    //添加报头
    system("clear");

    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
    //接收服务端发送过来的消息进行放序列化
    RecvReSerializeMsg(sockfd);
}

void ClientLogout(FirstRequset &msg, int sockfd) //注销
{
    cout << "------------------------------------" << endl;
    cout << "        请开始您的注销               " << endl;
    cout << "请输入您要注销的昵称 :";
    cin >> msg.nickname;
    cout << "请输入密码 :";
    cin >> msg.password;
    msg.logstatus = LOGINBEFORE;

    string req = FirRequsetSerialize(msg);
    system("clear");

    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
    RecvReSerializeMsg(sockfd);
}

void ClientQuit(FirstRequset &msg, int sockfd) //退出
{
    msg.logstatus = LOGINBEFORE;

    string req = FirRequsetSerialize(msg);    //服务器帮助退出
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
    RecvReSerializeMsg(sockfd);

    cout << "------------------------------------" << endl;
    cout << "           欢迎下次使用              " << endl;
    cout << "------------------------------------" << endl;
    sleep(2);
    system("clear");
    exit(1); //还是可以使用
}