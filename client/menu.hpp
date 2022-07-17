#pragma once
#include <iostream>
using namespace std;
#include "protocol.hpp"
#include"clientopt.hpp"
#include<cstdlib>

void menu(int sockfd,FirstRequset& msg)
{
    
    cout << "    Welcome      to     ChatRoom    " << endl;
    cout << "------------------------------------" << endl;
    cout << "     Enjoy              Yourself    " << endl;
    cout << "------------------------------------" << endl;
    cout << "     [1]登录           [2]注册       " << endl;
    cout << "     [3]注销           [4]退出       " << endl;
    cout << "------------------------------------" << endl;
    cout << "    Please Input Your Choice        " << endl;
    cin >> msg.type;//输入第一次操作类型
    system("clear");
    switch (msg.type)
    {
    case LOGIN:
        ClientLogin(msg,sockfd);
        break;
    case REGISTER:
        ClientRegister(msg,sockfd);
        break;
    case LOGOUT:
        ClientLogout(msg,sockfd);
        break;
    case QUIT:
        ClientQuit(msg,sockfd);//退出进程
        break;
    }
}