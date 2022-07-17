#pragma once
#include <cstdlib>
#include "protocol.hpp"
#include "jsonmsg.hpp"
void ClientLogin(FirstRequset &msg, int sockfd) //登录
{
    cout << "------------------------------------" << endl;
    cout<<"         请开始您的登录"<<endl;
    cout << "------------------------------------" << endl;
    cout<<"请输入您的昵称 :";
    cin>>msg.nickname;
    cout << "请输入您的密码 :";
    cin>>msg.password;
    string req = FirRequsetSerialize(msg);
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
    // FirstResponse recvmsg;


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

    //进行序列化，把消息发送给服务器
    string req = FirRequsetSerialize(msg);
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作

    FirstResponse recvres;
    char buf[MAX_SIZE];
    string tmp=RecvMsg(sockfd,buf,sizeof(buf)-1);

    FirResponseReSerialize(tmp,recvres);//进行反序列化
    if(recvres.status==SUCCESS)
    {
        cout<<recvres.msg<<endl;
    }
    else if(recvres.status==Failure)
    {
        cout<<recvres.msg<<endl;
    }
}
void ClientLogout(FirstRequset &msg, int sockfd) //注销
{
    cout << "------------------------------------" << endl;
    cout << "        请开始您的注销               " << endl;
    cout << "请输入您要注销的昵称 :";
    cin >> msg.nickname;
    cout << "请输入密码 :";
    cin >> msg.password;
    string req = FirRequsetSerialize(msg);
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
}

void ClientQuit(FirstRequset &msg,int sockfd) //退出
{
    cout << "------------------------------------" << endl;
    cout << "           欢迎下次使用              " << endl;
    cout << "------------------------------------" << endl;
    string req = FirRequsetSerialize(msg); //服务器帮助退出
    send(sockfd, req.c_str(), req.size(), 0); //发送给服务器进行操作
    sleep(2);
    system("clear");
    exit(1);//还是可以使用
}