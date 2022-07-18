#pragma once
#include <iostream>
#include <pthread.h>
#include "unistd.h"
#include <cstring>
#include "epoll.hpp"
#include "IO.hpp"
#include "protocol.hpp"
#include "jsonmsg.hpp"
#include"Myredis.hpp"

using namespace std;

namespace ns_task
{
    class Task
    {
    private:
        int _sockfd;
        int _epollfd;
        Epoll *_epl;
        Myredis _mrs;

    public:
        Task() //无参构造，为了拿任务，不需要参数列表
            : _sockfd(-1), _epollfd(-1), _epl(nullptr)
        {
        }
        //进行函数重载
        Task(int sockfd, int epollfd, Epoll *epl)
            : _sockfd(sockfd), _epollfd(epollfd), _epl(epl)
        {
        }

        ~Task()
        {
        }
        void ServerLogin(FirstRequset &req, int _sockfd)
        {
            //打开数据库，将账号和密码，进行匹配，成功返回进入下一步，失败返回错误
            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "            登录成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);

        }
        void ServerRegister(FirstRequset &req, int _sockfd)
        {
            //打开数据库，把数据存进去
            //所有用户的表添加，这个用户的表添加
            _mrs.connect();
            
            string buf="sadd Alluser "+req.nickname;
            
            _mrs.DoCommand(buf);
            //2.注册一个我这个用户的表
            buf="hmset "+req.nickname+" nickname "+req.nickname+" password "+req.password;
            _mrs.DoCommand(buf);
            //注册成功
            _mrs.disconnect();

            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "            注册成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }
        void ServerLogout(FirstRequset &req, int _sockfd)
        {
            //打开数据库，把对应的数据删除
            _mrs.connect();
            char buf[MAX_SIZE]={0};
            memset(buf,0,sizeof(buf));
            sprintf(buf,"srem AllUsr %s",req.nickname.c_str());
            _mrs.DoCommand(buf);
            _mrs.disconnect();

            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "            注销成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }
        void ServerQuit(FirstRequset &req, int _sockfd)
        {
            //退出
            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "            退出成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }
        int Run() //执行任务
        {

            string str = "";
            char buf[1024];
            memset(buf, 0, sizeof(buf));

            //这就是读的主要逻辑,第一次读
            recv_from_client(_sockfd, buf, str, _epollfd, _epl);

            if (str.size())
                str.pop_back();
            // fflush(stdin);

            //对第一次读取到的东西进行反序列化
            FirstRequset req;
            FirRequsetReSerialize(str, req);
            switch (req.type)
            {
            case LOGIN:
                ServerLogin(req, _sockfd);
                break;
            case REGISTER:
                ServerRegister(req, _sockfd);
                break;
            case LOGOUT:
                ServerLogout(req, _sockfd);
                break;
            case QUIT:
                ServerQuit(req, _sockfd); //退出进程
            }
            // cout << str << endl;

            // //这里就处理客户端发来的任务

            // send(_sockfd, str.c_str(), str.size(), 0);
        }

        int operator()() //重载一个仿函数
        {
            return Run();
        }
    };
}