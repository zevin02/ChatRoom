#pragma once
#include <iostream>
#include <pthread.h>
#include "unistd.h"
#include <cstring>
#include"epoll.hpp"
#include"IO.hpp"
using namespace std;

namespace ns_task
{
    class Task
    {
    private:
        int _sockfd;
        int _epollfd;
        Epoll* _epl;
    public:
        Task() //无参构造，为了拿任务，不需要参数列表
            : _sockfd(-1)
            ,_epollfd(-1)
            ,_epl(nullptr)
        {
        }
        //进行函数重载
        Task(int sockfd,int epollfd,Epoll* epl)
            : _sockfd(sockfd),_epollfd(epollfd),_epl(epl)
        {
        }

        ~Task()
        {
        }
        int Run() //执行任务
        {

            string str = "";
            char buf[1024];
            memset(buf, 0, sizeof(buf));

            //这就是读的主要逻辑
            recv_from_client(_sockfd, buf, str, _epollfd, _epl);

            if (str.size())
                str.pop_back();
            // fflush(stdin);
            cout << str << endl;

            //这里就处理客户端发来的任务

            send(_sockfd, str.c_str(), str.size(), 0);

            
        }

        int operator()() //重载一个仿函数
        {
            return Run();
        }
    };
}