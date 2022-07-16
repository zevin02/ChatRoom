
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstdio>
#include<unistd.h>
#include"client.hpp"
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
    Client* clt=new Client(ip,port);
    clt->InitClient();
    clt->StartClient();
    delete clt;
 
    return 0;
}