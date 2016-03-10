//packet.cpp
#include <arpa/inet.h>
#include <unistd.h>

#include "helpers.h"
#include "socket.h"
int pid =getpid();
icmphdr CreateIcmpHeader(int orderNumber)
{
    struct icmphdr IcmpHeader;
    IcmpHeader.type = ICMP_ECHO;
    IcmpHeader.code=0;
    IcmpHeader.un.echo.id=pid;
    IcmpHeader.un.echo.sequence=orderNumber;
    IcmpHeader.checksum=0;
    IcmpHeader.checksum = compute_icmp_checksum((u_int16_t*)&IcmpHeader,sizeof(IcmpHeader));
    return IcmpHeader;
}
sockaddr_in CreateSocketConnection()
{
    struct sockaddr_in Rec;
    bzero(&Rec, sizeof(Rec));
    Rec.sin_family=AF_INET;
    inet_pton(AF_INET,"8.8.8.8", &Rec.sin_addr);
    return Rec;
}


void Send (int ttl,int socfd)
{
    for (int i=0;i<3;i++)
    {
        int orderNumber = ttl*10+i;
        printf("Sending packets with oN %d\n",orderNumber);
        icmphdr IcmpHeader = CreateIcmpHeader(orderNumber);
        sockaddr_in Rec = CreateSocketConnection();
        ChangeTTL(socfd,ttl);
        SendPacket(socfd,IcmpHeader,Rec);
    }
}
