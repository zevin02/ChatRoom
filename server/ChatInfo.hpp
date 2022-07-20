#pragma once
#include <iostream>
#include <list>
#include <string>
#include"Myredis.hpp"
using namespace std;

struct User// 在线用户信息
{
    int _fd;//在线用户对应的fd
    string _name;//用户名
    User(int fd,string name)
    :_fd(fd),_name(name)
    {}
};

struct Groupuser //群成员信息
{
    string name;
};
struct Group
{
    string name;     //群名
    list<Groupuser> l; //群成员
};

class ChatInfo
{
private:
    list<User> online_user; //在线用户信息，用指针来保存
    // list<Group> group_info; //保存所有的群信息
    // Myredis* mrs;
public:
    ChatInfo()
    {
        // online_user=new list<User>;//初始化用户链表,这个用户链表我们只需要一个空的即可
        // group_info=new list<Group>;//初始化群信息，这个群信息，我们就需要去数据库里面读取，把所有的群信息加载进来
        // mrs=new Myredis;//数据库是用来读取所有的群信息
        //当有用户
        
    }
    void User_Pushback(string name,int fd)//用户信息链接上去
    {
        bool flag=false;
        User newman(fd,name);
        //先去里面找，如果找到了同名的就不插入，如果没找到同名的就插入
        auto it=online_user.begin();
        while(it!=online_user.end())//循环走一遍，发现的
        {
            if(it->_name==name)
            {
                flag=true;
            }
            it++;
        }
        if(!flag)//走一圈发现没有被添加进去过，就添加进去
        online_user.push_back(newman);//添加用户到在线用户链表里面
        
    }
    void User_Erase(string name)
    {
        auto it=online_user.begin();
        while(it!=online_user.end())
        {
            if(it->_name==name)
            {
                break;
            }
            it++;
        }
        cout<<__FILE__<<": "<<__LINE__<<" : "<<(*it)._name<<endl;
        online_user.erase(it);//下线就把对应的节点删除
    }
    void PrintOnlineUser()
    {
        cout<<"online user number"<<online_user.size()<<endl;
        auto it=online_user.begin();
        while(it!=online_user.end())
        {
        cout<<__FILE__<<": "<<__LINE__<<" name : "<<(*it)._name<<" fd : "<<(*it)._fd<<endl;
        it++;
        }
    }
    ~ChatInfo()
    {
        // delete online_user;
        // delete group_info;
        // delete mrs;
    }

};