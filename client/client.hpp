#pragma once
#include <string>
#include <iostream>
#include "sock.hpp"
#include"IO.hpp"
#include"menu.hpp"

using namespace std;
class Client
{
private:
    string _ip;
    int _port;
    int _sockfd;
public:
    Client(string ip, uint16_t port)
        : _ip(ip), _port(port),_sockfd(-1)
    {
    }
    ~Client()
    {
        close(_sockfd);
    }
    void InitClient()
    {
        _sockfd=Sock::Socket();
        Sock::Connect(_sockfd,_ip,_port);

    }
    void StartClient()
    {
        FirstRequset msg;
        do
        {
            menu(_sockfd,msg);

        }while(msg.type);
            // string str;
            // getline(cin,str);
            // send(_sockfd,str.c_str(),str.size(),0);
            // char buf[1024];
            //登录界面



            // string recv_msg=RecvMsg(_sockfd,buf,sizeof(buf)-1);
            // cout<<recv_msg<<endl;
        
    }
};