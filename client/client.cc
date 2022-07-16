#include<iostream>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstdio>
#include<unistd.h>
using namespace std;


int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        cout<<"ip+port"<<endl;
        return 1;
    }
    string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cerr<<"socket error"<<endl;
        return 2;
    }
    struct sockaddr_in svr;
    memset(&svr,0,sizeof(svr));
    svr.sin_family=AF_INET;
    svr.sin_port=htons(port);
    svr.sin_addr.s_addr=inet_addr(ip.c_str());
    if(connect(sockfd,(struct sockaddr*)&svr,sizeof(svr))<0)
    {
        cerr<<"连接失败"<<endl;
        return 3;
    }
    cout<<"connect success"<<endl;

    //这里就可以进行正常的业务请求了
    while(true)
    {
        cout<<"please enter ";
        char buf[1024];
        fgets(buf,sizeof(buf)-1,stdin);
        write(sockfd,buf,strlen(buf));
        char bufrecv[1024];
        ssize_t s=read(sockfd,bufrecv,sizeof(bufrecv)-1);
        if(s>0)
        {
            bufrecv[s]=0;
            cout<<"recv "<<bufrecv<<endl;
        }
        else
        {
            cout<<"error"<<endl;
        }

    }
    return 0;
}