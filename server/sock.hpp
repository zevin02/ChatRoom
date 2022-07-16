#pragma once
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<cstdlib>
#include<arpa/inet.h>
#include<cstring>
#include<string>
using namespace std;



class Sock
{
    public:
    static int Socket()
    {
        int sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(sockfd<0)
        {
            perror("socket");
            exit(1);
        }
        return sockfd;
    }
    
    static void Listen(int sockfd)
    {
        if(listen(sockfd,5)<0)
        {
            perror("listen");
            exit(1);
        }
    }
    static void Bind(int sockfd,int port,string ip)
    {
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(port);
        local.sin_addr.s_addr=inet_addr(ip.c_str());

        if(bind(sockfd,(struct sockaddr*)&local,sizeof(local))<0)
        {
            perror("bind");
            exit(1);
        }
    }
    static void SetSockOpt(int sockfd)
    {
        //端口复用
        int opt=1;
        setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
    }

    static int Accept(int sockfd)
    {
        struct sockaddr_in peer;
        memset(&peer,0,sizeof(peer));
        socklen_t len=sizeof(peer);
        int peerfd=accept(sockfd,(struct sockaddr*)&peer,&len);
        if(peerfd<0)
        {
            perror("accept");
            exit(2);
        }
        return peerfd;
    }
};



