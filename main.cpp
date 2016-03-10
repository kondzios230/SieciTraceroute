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
using namespace std;
int NumberOfHops =30;

int main(int argc, char** argv)
{
    Validate(argc,argv[1]);
    int socfd = CreateSocket();
    for(int i=1;i<=NumberOfHops;i++)
    {
        Send(i,socfd);

    }

        return 0;
}

