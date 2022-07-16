#include "sock.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include<glog/logging.h>

void addfd(int epollfd, int sockfd)
{
    struct epoll_event eve;
    eve.data.fd = sockfd;
    eve.events = EPOLLIN | EPOLLET;
    //设置非阻塞
    int flag = fcntl(sockfd, F_GETFL,0);
    fcntl(sockfd, F_SETFL,flag | O_NONBLOCK);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &eve);
}

void et(struct epoll_event *events, int ret, int epollfd, int sockfd)
{
    char buf[1024];
    for (int i = 0; i < ret; i++)
    {
        if (events[i].data.fd == sockfd)
        {
            int newfd = Sock::Accept(sockfd);
            addfd(epollfd, newfd); //把这个新来的fd添加进去
        }
        else if (events[i].events & EPOLLIN)
        {
            //不是读事件
            //这段代码不会被重复触发，因为我们设置的是et模式，非阻塞，所以我们要循环读取
            //因为它不会二次触发数据，所以就要求我们一次性把数据都读完，我们只能循环，非阻塞，没有数据的时候退出就行了
            string str = "";
            //这就是读的主要逻辑
            while (1)
            {
                memset(buf, 0, sizeof(buf));
                ssize_t s = recv(events[i].data.fd, buf, 4, 0);
                // ssize_t s=read(events[i].data.fd,buf,666);
                if (s < 0) //<0不算错误，
                {
                    //对于非阻塞IO，当下面的条件成立，才算真正的读取完全,下一次epoll就能再次触发sockfd上面的epollin事件，进行下一次操作，而不是还没读完，又触发一遍

                    // EAGAIN表示try again，表示底层数据没有准备好，下次再来，EWOULDBLOCK也是一样的效果，在一些平台上，他们的值一样的

                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        cout << "read later" << endl;
                        break;
                    }
                    cout << "time out" << endl;
                    close(events[i].data.fd);
                    int fd = events[i].data.fd;
                    struct epoll_event ev;
                    ev.data.fd = fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
                    break;
                }
                else if (s == 0)
                {
                    close(events[i].data.fd); //对端关闭，直接关闭
                    //关闭之后就要把它从树上调出去
                    int fd = events[i].data.fd;
                    struct epoll_event ev;
                    ev.data.fd = fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
                    cout << "client quit" << endl;
                    break;
                }
                else
                {
                    fflush(stdin);
                    str += buf;
                    fflush(stdin);
                    // cout<<str;
                    // fflush(stdout);
                    // cout<<buf;//这样就不会因为缓冲区设置的问题，而导致片段访问
                }
            }
            if(str.size())
            str.pop_back();
            // fflush(stdin); 
            cout << str<<endl;
            
            str+=" client hello";
            send(events[i].data.fd,str.c_str(),str.size(),0);
            // cout << endl; //循环结束之后，再回车即可
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }
    int sockfd = Sock::Socket();
    Sock::SetSockOpt(sockfd);
    Sock::Bind(sockfd, atoi(argv[1]), "127.0.0.1");
    Sock::Listen(sockfd);
    int epollfd = epoll_create(1);
    addfd(epollfd, sockfd);
    struct epoll_event events[10];
    while (1)
    {
        int ret = epoll_wait(epollfd, events, 10, -1); //这里我们设置成阻塞状态
        if (ret < 0)
        {
            break;
        }
        et(events, ret, epollfd, sockfd);
    }
    return 0;
}