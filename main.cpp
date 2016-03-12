#include <iostream>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket.h"
#include "helpers.h"
#include "packet.h"

#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
int pid =getpid();
using namespace std;
int NumberOfHops =30;





int main(int argc, char** argv)
{
    //Validate(argc,argv[1]);
    char* ip ="8.8.8.8";
    int socfd = CreateSocket();
    for(int i=1;i<=NumberOfHops;i++)
    {
        Send(i,socfd,pid,ip);
        printf("TTL %d\n",i);
        Recieve(socfd,i);
    }

        return 0;
}
