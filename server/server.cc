#pragma
#include "IO.hpp"
#include"server.hpp"
#include"head.hpp"
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }
 
    uint16_t port=atoi(argv[1]);
    Server* svr=new Server(port);
    svr->InitServer();
    svr->StartServer();
    delete svr;
    return 0;
}