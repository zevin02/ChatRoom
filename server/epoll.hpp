#pragma once
#include "head.hpp"
class Epoll
{
private:
    int epollfd;

public:
    Epoll()
        : epollfd(-1)
    {
    }
    ~Epoll()
    {
    }
    int Creat(int num)
    {
        epollfd = epoll_create(num);
        return epollfd;
    }
    void DELFd(int sockfd) //删除一个句柄
    {
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);
    }
    void AddFdNoBlock(int sockfd) //添加一个句柄,设置非阻塞
    {
        struct epoll_event eve;
        eve.data.fd = sockfd;
        eve.events = EPOLLIN | EPOLLET;
        //设置非阻塞
        int flag = fcntl(sockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
        epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &eve);
    }
    int Wait(struct epoll_event *event)
    {
        int ret = epoll_wait(epollfd, event, 10, -1);
        if (ret < 0)
        {
            perror("epoll_wait");
            exit(1);
        }
        return ret;
    }
  
};