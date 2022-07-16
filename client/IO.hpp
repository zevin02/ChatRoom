#pragma once
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std;
string RecvMsg(int sockfd,char* buf,int size)//这个主要是客户端使用,接收服务器发送给客户端的信息
{
    ssize_t s=recv(sockfd,buf,size,0);
    if(s>0)
    {
        buf[s]=0;
        cout<<"recv "<<buf<<endl;
    }
    else 
    {
        cout<<"error"<<endl;

    }
    string msg=buf;
    return msg;
}
