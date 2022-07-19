#pragma once
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"protocol.hpp"
#include"jsonmsg.hpp"
using namespace std;
string RecvMsg(int sockfd,char* buf,int size)//这个主要是客户端使用,接收服务器发送给客户端的信息
{
    ssize_t s=recv(sockfd,buf,size,0);
    if(s>0)
    {
        buf[s]=0;
    }
    else 
    {
        cout<<"error"<<endl;

    }
    string msg=buf;
    return msg;
}

void RecvReSerializeMsg(int sockfd)//接收服务端发送过来的消息进行反序列化 
{
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