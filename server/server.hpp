#include "head.hpp"
#include "threadpool.hpp"
using namespace ns_threadpool;
#include "Task.hpp"
using namespace ns_task;
#include"ChatInfo.hpp"
class Server
{
private:
    int _epollfd;
    int _sockfd;
    int _port;
    Epoll *epl;
    ChatInfo* chatinfo;//里面包含一个在线用户链表
public:
    Server(int port)
        : _epollfd(-1), _sockfd(-1), _port(port),epl(nullptr),chatinfo(nullptr)
    {
    }
    ~Server()
    {
        delete epl;
        delete chatinfo;
    }
    void InitServer()
    {
        epl = new Epoll();
        chatinfo=new ChatInfo();
        _epollfd = epl->Creat(10);
        _sockfd = Sock::Socket();
        Sock::SetSockOpt(_sockfd);
        Sock::Bind(_sockfd, _port);
        Sock::Listen(_sockfd);
    }

    void EpollEt(struct epoll_event *events, int ret)
    {
        char buf[1024];
        for (int i = 0; i < ret; i++)
        {
            int newsock = events[i].data.fd;
            if (newsock == _sockfd)
            {
                int newfd = Sock::Accept(_sockfd);
                epl->AddFdNoBlock(newfd); //把这个新来的fd添加进去
            }
            else if (events[i].events & EPOLLIN) //处理任务，我们就放到线程池里面处理
            {
                //不是读事件
                //这段代码不会被重复触发，因为我们设置的是et模式，非阻塞，所以我们要循环读取
                //因为它不会二次触发数据，所以就要求我们一次性把数据都读完，我们只能循环，非阻塞，没有数据的时候退出就行了
                Task t(newsock, _epollfd, epl,chatinfo);
                ThreadPool<Task>::GetInstance()->PushTask(t); //单例模式
            }
        }
    }
    void StartServer()
    {
        epl->AddFdNoBlock(_sockfd);
        struct epoll_event events[10];
        while (1)
        {
            int ret = epl->Wait(events);
            EpollEt(events, ret);
        }
    }
};