#pragma once
#include <iostream>
#include <pthread.h>
#include "unistd.h"
#include <cstring>
#include "epoll.hpp"
#include "IO.hpp"
#include "protocol.hpp"
#include "jsonmsg.hpp"
#include <glog/logging.h>
#include "ChatInfo.hpp"
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
        ChatInfo *_chatinfo; //里面有两个链表
    public:
        Task() //无参构造，为了拿任务，不需要参数列表
            : _sockfd(-1), _epollfd(-1), _epl(nullptr), _chatinfo(nullptr)
        {
        }
        //进行函数重载
        Task(int sockfd, int epollfd, Epoll *epl, ChatInfo *chatinfo)
            : _sockfd(sockfd), _epollfd(epollfd), _epl(epl), _chatinfo(chatinfo)
        {
        }

        ~Task()
        {
        }

        //登录之后的操作

        void ServerLogin(FirstRequset &req, int _sockfd) //登录
        {
            //打开数据库，将账号和密码，进行匹配，成功返回进入下一步，失败返回错误
            FirstResponse rep;
            _mrs.connect();
            string buf = "sismember Alluser " + req.nickname;
            if (_mrs.isExist(buf))
            {
                // 我们输入的这个用户名存在，那么就继续处理，对密码进行验证，验证匹配，就删除
                buf = "hget " + req.nickname + " password";
                string realpassword = _mrs.GetAData(buf);
                if (req.password == realpassword)
                {
                    buf = "srem Alluser " + req.nickname;
                    _mrs.disconnect();
                    rep.status = SUCCESS;
                    rep.msg = "            登录成功";
                    _chatinfo->User_Pushback(req.nickname, _sockfd); //用户上线之后就发送过去了
                    _chatinfo->PrintOnlineUser();
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
        void ServerRegister(FirstRequset &req, int _sockfd) //注册账号
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
        void ServerLogout(FirstRequset &req, int _sockfd) //注销账号
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
                string realpassword = _mrs.GetAData(buf);
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
        void ServerQuit(FirstRequset &req, int _sockfd) //退出进程
        {
            //退出
            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "            退出成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void ServerFRIENDADD(FirstRequset &req, int _sockfd) //添加好友，双方的好友数据库都要增加
        {
            FirstResponse rep;
            //在注册的用户里面去找，找到了添加，每找到，返回失败
            _mrs.connect();
            //先检测是否存在,在Alluser里面检查
            string buf = "sismember Alluser " + req.tonickname;
            if (_mrs.isExist(buf))
            {
                //存在就添加进去
                buf = "sadd " + req.nickname + "_friend " + req.tonickname; //
                _mrs.AddData(buf);
                buf = "sadd " + req.tonickname + "_friend " + req.nickname; //
                _mrs.AddData(buf);
                _mrs.disconnect();
                rep.status = SUCCESS;
                rep.msg = "      添加成功";
            }
            else
            {
                rep.status = Failure;
                rep.msg = "      该用户不存在";
            }
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void ServerFRIENDDEL(FirstRequset &req, int _sockfd) //删除好友
        {
            FirstResponse rep;
            //在注册的用户里面去找，找到了添加，每找到，返回失败
            _mrs.connect();
            //先检测要删除的用户是否存在,在nickname_friend里面找
            string buf = "sismember " + req.nickname + "_friend " + req.tonickname;
            if (_mrs.isExist(buf))
            {
                //存在就添加进去
                buf = "srem " + req.nickname + "_friend " + req.tonickname;
                _mrs.DelData(buf);
                buf = "srem " + req.tonickname + "_friend " + req.nickname;
                _mrs.DelData(buf);
                _mrs.disconnect();
                rep.status = SUCCESS;
                rep.msg = "      删除成功";
            }
            else
            {
                rep.status = Failure;
                rep.msg = "      该好友不存在";
            }
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void FriendCheckMember(int sockfd, FirstRequset &req) //查看有多少个好友
        {
            FirstResponse rep;
            _mrs.connect();
            string buf = "smembers " + req.nickname + "_friend";

            rep.msg = _mrs.GetVectorString(buf);

            _mrs.disconnect();
            rep.status = SUCCESS;
            string msg = FirstResponseSerialize(rep);

            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void ServerLOADEXIT(FirstRequset &req, int sockfd, ChatInfo *_chatinfo) //退出登录
        {
            _chatinfo->User_Erase(req.nickname); //把这个节点删除掉
            FirstResponse rep;
            rep.status = SUCCESS;
            rep.msg = "          退出登录成功";
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void ServerFRIENDCHECKONLINE(FirstRequset &req, int _sockfd, ChatInfo *_chatinfo) //查看好友是否在线
        {
            //在在线用户链表里面找，找到就说明有在线，没找到说明不在线
            FirstResponse rep;
            if (_chatinfo->IsExist(req.tonickname))
            {
                //找到了
                rep.status = SUCCESS;
                rep.msg = "         该好友在线";
            }
            else
            {
                rep.status = Failure;
                rep.msg = "       该好友不在线";
            }
            string msg = FirstResponseSerialize(rep);
            send(_sockfd, msg.c_str(), msg.size(), 0);
        }

        void ServerFriendCHATONLINE(FirstRequset &req, int _sockfd, ChatInfo *_chatinfo) //与在线好友进行聊天
        {

            FirstResponse rep;
            //先判断是否在线，如果在线的话就直接发送了
            if (_chatinfo->IsExist(req.tonickname))
            {
                //用户在线
                //先给对方发送，谁要和你进行聊天
                // rep.msg = req.nickname + " wanna chat with you";//这个我们只希望第一次想要和它聊天的时候发送
                rep.msg = req.nickname + " : " + req.message;
                rep.status = SUCCESS;
                string msg = FirstResponseSerialize(rep);
                int friendfd = _chatinfo->GetFriendFd(req.tonickname);

                cout << __FILE__ << __LINE__ << "对方的fd为: " << friendfd << "  " << msg << endl;
                send(friendfd, msg.c_str(), msg.size(), 0);
            }
            else
            {
                //用户不在线，我们就发送给数据库，等到对方上线之后先去读取这个
                //这里我们把数据修改一下
                string buff = req.nickname + " send message to you : " + req.message; //这个就是给没上线的好友发送的消息
                _chatinfo->UnReadBuf_Pushback(req.tonickname, buff);                  //不管对方有没有上线，只要往这里面加数据就可以了
                cout << "不在线" << endl;
            }
        }

        
        void ServerCheckUnReadMsg(FirstRequset &req, int _sockfd, ChatInfo *_chatinfo) //查看未读取消息
        {
            FirstResponse rep;
            //进去之后先去用户未读缓冲区里面查看
            int size = _chatinfo->UnReadMsgNum(req.nickname);
            rep.msg = "您有" + to_string(size) + "条消息\n";
            rep.status = SUCCESS;
            if (size)
            {
                while (size--)
                {
                    rep.msg += _chatinfo->GetUnReadMsg(req.nickname);
                    rep.msg += "\n";
                }
                cout << __FILE__ << __LINE__ << rep.msg << endl;
                string msg = FirstResponseSerialize(rep);
                send(_sockfd, msg.c_str(), msg.size(), 0); //先告诉服务器有几条消息
            }
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
            else if (req.logstatus == LOGINAFTER) //这里就是用户登录成功之后
            {

                // _chatinfo->PrintOnlineUser();
                req.fdfrom = _sockfd;
                cout << __FILE__ << __LINE__ << " req.fd=" << req.fdfrom << endl;
                switch (req.type)
                {
                case CHECKUNREADMESSAGE:
                    ServerCheckUnReadMsg(req, _sockfd, _chatinfo);
                    break;
                case FRIEND_CHECK_MEMBER:
                    FriendCheckMember(_sockfd, req);
                    break;
                case FRIEND_ADD:
                    ServerFRIENDADD(req, _sockfd);
                    break;
                case FRIEND_DEL:
                    ServerFRIENDDEL(req, _sockfd);
                    break;
                case FRIEND_CHAT_ONLINE:
                    // ServerFriendCHATONLINE(req, _sockfd, _chatinfo);
                    // break;
                case FRIEND_CHAT_UNONLINE:
                    ServerFriendCHATONLINE(req, _sockfd, _chatinfo);
                    break;
                case FRIEND_CHECK_ONLINE:
                    ServerFRIENDCHECKONLINE(req, _sockfd, _chatinfo);
                    // ServerFriendCHATUNONLINE(req, _sockfd, _chatinfo);
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
                case LEFTLOAD:
                    cout << "quit" << endl;
                    //在这里把对应的用户数据从里面删除掉
                    ServerLOADEXIT(req, _sockfd, _chatinfo);
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