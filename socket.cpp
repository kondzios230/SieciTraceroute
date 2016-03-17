//socket.cpp
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket.h"
#include <errno.h>
#include <arpa/inet.h>
#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }
int CreateSocket()
{
    int	n;
    if ( (n = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) < 0)
        ERROR ("Error while creating an socket");
    return n;
}
void SendPacket(int socket,icmphdr IcmpHeader, sockaddr_in Rec)
{
    if (sendto(socket,&IcmpHeader,sizeof(IcmpHeader),0,(struct sockaddr*)&Rec,sizeof(Rec)) != sizeof(IcmpHeader))
        ERROR ("Error while sending an packet");
}
void ChangeTTL(int socket,int ttl)
{
    int TTL= ttl;
    if ( setsockopt(socket,IPPROTO_IP,IP_TTL,&TTL,sizeof(int)) < 0)
        ERROR ("Error while changing TTL");
}
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    int n;
    if ((n = select(nfds,readfds,writefds,exceptfds,timeout)) < 0)
        ERROR ("Error while waiting for socket");
    return n;
}
ssize_t Recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr)
{
    ssize_t	n;
    if ( (n = recvfrom(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salenptr)) < 0)
        ERROR ("Error while recieving from socket");
    return n;
}
sockaddr_in CreateSocketConnection(char * ip)
{
    struct sockaddr_in Rec;
    bzero(&Rec, sizeof(Rec));
    Rec.sin_family=AF_INET;
    inet_pton(AF_INET,ip, &Rec.sin_addr);
    return Rec;
}
