#pragma once
#include <iostream>
using namespace std;
#include"head.hpp"

//这里不能包含server.hpp

void recv_from_client(int newsock, char *buf, string& str, int epollfd,Epoll* epl)
{
    while (1)
    {
        ssize_t s = recv(newsock, buf, 1024, 0);
        // ssize_t s=read(events[i].data.fd,buf,666);
        if (s < 0) //<0不算错误，
        {
            //对于非阻塞IO，当下面的条件成立，才算真正的读取完全,下一次epoll就能再次触发sockfd上面的epollin事件，进行下一次操作，而不是还没读完，又触发一遍

            // EAGAIN表示try again，表示底层数据没有准备好，下次再来，EWOULDBLOCK也是一样的效果，在一些平台上，他们的值一样的

            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            close(newsock);
            epl->DELFd(newsock);
            break;
        }
        else if (s == 0)
        {
            close(newsock); //对端关闭，直接关闭
            //关闭之后就要把它从树上调出去
            epl->DELFd(newsock);
            cout << "client quit" << endl;
            break;
        }
        else
        {
            fflush(stdin);
            str += buf;
            fflush(stdin);
            
        }
    }
}
