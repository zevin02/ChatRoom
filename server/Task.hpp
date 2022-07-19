#pragma once
#include <iostream>
#include <pthread.h>
#include "unistd.h"
#include <cstring>
#include "epoll.hpp"
#include "IO.hpp"
#include "protocol.hpp"
#include "jsonmsg.hpp"
#include "Myredis.hpp"
#include <glog/logging.h>
using namespace std;

namespace ns_task
{
    class Task
    {
    private:
        int _sockfd; //发送者的fd
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

        //登录之后的操作

        void ServerLogin(FirstRequset &req, int _sockfd)
        {
            //打开数据库，将账号和密码，进行匹配，成功返回进入下一步，失败返回错误
            FirstResponse rep;
            _mrs.connect();
            string buf = "sismember Alluser " + req.nickname;
            if (_mrs.isExist(buf))
            {
                // 我们输入的这个用户名存在，那么就继续处理，对密码进行验证，验证匹配，就删除
                buf = "hget " + req.nickname + " password";
                string realpassword = _mrs.GetData(buf);
                if (req.password == realpassword)
                {
                    buf = "srem Alluser " + req.nickname;
                    _mrs.disconnect();
                    rep.status = SUCCESS;
                    rep.msg = "            登录成功";
                }
                else
                {
                    rep.status = Failure;
                    rep.msg = "            登录失败";
                }
            }
            else
            {
                //对应的用户名不存在
                rep.status = Failure;
                rep.msg = "            登录失败";
            }

            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);

            cout << __FILE__ << ":" << __LINE__ << "行" << rep.status << endl;
        }
        void ServerRegister(FirstRequset &req, int _sockfd)
        {
            //打开数据库，把数据存进去
            //所有用户的表添加，这个用户的表添加
            _mrs.connect();

            string buf = "sadd Alluser " + req.nickname;

            _mrs.AddData(buf);
            // 2.注册一个我这个用户的表
            buf = "hmset " + req.nickname + " nickname " + req.nickname + " password " + req.password;
            _mrs.AddData(buf);
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
            //输入要删除的id和密码，相同才能删除
            //同时把它的数据库也删除掉
            FirstResponse rep;

            _mrs.connect();
            string buf = "sismember Alluser " + req.nickname;
            if (_mrs.isExist(buf))
            {
                // 我们输入的这个用户名存在，那么就继续处理，对密码进行验证，验证匹配，就删除
                buf = "hget " + req.nickname + " password";
                string realpassword = _mrs.GetData(buf);
                if (req.password == realpassword)
                {
                    //删除数据库中Alluser里面的nickname
                    buf = "srem Alluser " + req.nickname;
                    _mrs.DelData(buf);
                    _mrs.disconnect();
                    rep.status = SUCCESS;
                    rep.msg = "            注销成功";
                }
                else
                {
                    rep.status = Failure;
                    rep.msg = "            注销失败";
                }
            }
            else
            {
                //对应的用户名不存在
                rep.status = Failure;
                rep.msg = "            注销失败";
            }

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

        void ServerFRIENDADD(FirstRequset &req, int _sockfd) //添加好友
        {
            FirstResponse rep;
            //在注册的用户里面去找，找到了添加，每找到，返回失败
            _mrs.connect();
            //先检测是否存在,在Alluser里面检查
            string buf = "sismember Alluser " + req.tonickname;
            if (_mrs.isExist(buf))
            {
                //存在就添加进去
                buf = "sadd " + req.nickname + "_friend " + req.tonickname;
                _mrs.AddData(buf);
                _mrs.disconnect();
                rep.status = SUCCESS;
                rep.msg = "      添加成功";
            }
            else
            {
                rep.status = Failure;
                rep.msg = "      该好友不存在";
            }
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        //服务器判断是哪一种接收格式
        int Run() //执行任务
        {

            string str = "";
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int req_type;

            //这就是读的主要逻辑,第一次读
            // recv_from_client(_sockfd, (char*)&req_type, str, _epollfd, _epl,4);//先读前4个字节

            recv_from_client(_sockfd, buf, str, _epollfd, _epl);

            if (str.size())
                str.pop_back();
            fflush(stdin);
            cout << __FILE__ << "接收数据" << __LINE__ << ":" << str << endl;
            // LOG(ERROR)<<str;
            //对第一次读取到的东西进行反序列化

            FirstRequset req;
            FirRequsetReSerialize(str, req);
            cout << req.tonickname << endl;
            if (req.logstatus == LOGINBEFORE)
            {
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
            }
            else if (req.logstatus == LOGINAFTER)
            {
                req.fdfrom = _sockfd;
                cout << __FILE__ << __LINE__ << " req.fd=" << req.fdfrom << endl;
                switch (req.type)
                {
                case FRIEND_CHECK_MEMBER:
                    // FriendCheckMember(_sockfd,req);
                    cout << __FILE__ << __LINE__ << "CHECK FRIEND LIST" << endl;
                    break;
                case FRIEND_ADD:
                    ServerFRIENDADD(req, _sockfd);

                    break;
                case FRIEND_DEL:
                    break;
                case FRIEND_CHAT:
                    break;
                case FRIEND_CHECK_ONLINE:
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
                }
            }

            //应该在这下面处理登录之后的操作
        }

        int operator()() //重载一个仿函数
        {
            return Run();
        }
    };
}