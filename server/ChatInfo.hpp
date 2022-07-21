#pragma once
#include <iostream>
#include <list>
#include <string>
#include "Myredis.hpp"
using namespace std;

struct User // 在线用户信息
{
    int _fd;      //在线用户对应的fd
    string _name; //用户名
    User(int fd, string name)
        : _fd(fd), _name(name)
    {
    }
};

struct Groupuser //群成员信息
{
    string name;
};
struct Group
{
    string name;       //群名
    list<Groupuser> l; //群成员
};

struct UnReadMsg //用户的未读缓冲区,上线的时候先查看这个缓冲区，看是否有消息
{
    string owner;            //给谁发送的
    queue<string> msg_queue; //发送的消息队列,这个信息里面要包含是谁发送的
    int fd;                  //上线之后，就有了fd就可以给它发送了这个消息

    UnReadMsg(string ownername)
        : owner(ownername)
    {
    }
};

class ChatInfo
{
private:
    list<User> online_user; //在线用户信息，用指针来保存
    // list<Group> group_info; //保存所有的群信息
    list<UnReadMsg> UnReadbuf; //未读缓冲区链表
    // Myredis* mrs;
public:
    ChatInfo()
    {
    }

    string GetUnReadMsg(string name)
    {
        //这个就说明它已经存在了
        auto it = UnReadbuf.begin();
        while (it != UnReadbuf.end())
        {
            if (it->owner == name)
            {
                //这个节点已近存在了，我们只要往消息队列里面放数据就可以了
                break;
            }
            it++;
        }
        string retmsg;
        if(!it->msg_queue.empty())
        {
            retmsg=it->msg_queue.front();
            cout<<__FILE__<<__LINE__<<retmsg<<endl;
            it->msg_queue.pop();
        }
        return retmsg;
    }
    int UnReadMsgNum(string &name)
    {
        // 1.有可能没有人给你发过消息，这这个节点不存在，有可能存在，但是没有人给发消息
        bool flag=false;
        auto it = UnReadbuf.begin();
        while (it != UnReadbuf.end())
        {
            if (it->owner == name)
            {
                //这个节点已近存在了，我们只要往消息队列里面放数据就可以了
                flag = true;
                break;
            }
            it++;
        }
        if(flag)
        {
            //说明有这个节点
            return it->msg_queue.size();//返回它对应的节点个数
        }
        else
        {
            return 0;//没有找到说明，没有人发送，直接放回0，就可以了
        }

    }
    
    void UnReadBuf_Pushback(string &name, string &msg)
    {
        bool flag = false;
        auto it = UnReadbuf.begin();
        while (it != UnReadbuf.end())
        {
            if (it->owner == name)
            {
                //这个节点已近存在了，我们只要往消息队列里面放数据就可以了
                flag = true;
                break;
            }
            it++;
        }
        if (flag)
        {
            //节点存在

            it->msg_queue.push(msg); //把数据放到消息队列里面
        }
        else
        {
            //节点不存在,就创建一个节点
            UnReadMsg info(name);
            info.msg_queue.push(msg); //把数据放进去
            UnReadbuf.push_back(info);
        }
    }
    void User_Pushback(string &name, int &fd) //用户信息链接上去
    {
        bool flag = false;
        User newman(fd, name);
        //先去里面找，如果找到了同名的就不插入，如果没找到同名的就插入
        auto it = online_user.begin();
        while (it != online_user.end()) //循环走一遍，发现的
        {
            if (it->_name == name)
            {
                flag = true;
            }
            it++;
        }
        if (!flag)                         //走一圈发现没有被添加进去过，就添加进去
            online_user.push_back(newman); //添加用户到在线用户链表里面
    }
    void User_Erase(string &name)
    {
        auto it = online_user.begin();
        while (it != online_user.end())
        {
            if (it->_name == name)
            {
                break;
            }
            it++;
        }
        cout << __FILE__ << ": " << __LINE__ << " : " << (*it)._name << endl;
        online_user.erase(it); //下线就把对应的节点删除
    }
    void PrintOnlineUser()
    {
        cout << "online user number" << online_user.size() << endl;
        auto it = online_user.begin();
        while (it != online_user.end())
        {
            cout << __FILE__ << ": " << __LINE__ << " name : " << (*it)._name << " fd : " << (*it)._fd << endl;
            it++;
        }
    }

    bool IsExist(string &name) //判断某一个用户受否在线
    {
        auto it = online_user.begin();
        while (it != online_user.end())
        {
            if (it->_name == name)
            {
                return true; //发现了就返回真
            }
            it++;
        }
        return false; //没发现就返回假
    }
    int GetFriendFd(string &name)
    {
        auto it = online_user.begin();
        while (it != online_user.end())
        {
            if (it->_name == name)
            {
                break;
            }
            it++;
        }
        return it->_fd;
    }
    ~ChatInfo()
    {
    }
};