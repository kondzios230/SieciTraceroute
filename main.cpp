#include <iostream>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }
using namespace std;
u_int16_t compute_icmp_checksum (const u_int16_t *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}
int CreateSocket()
{
    int	n;
	if ( (n = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) < 0)
		ERROR ("Error while creating an socket");
	return n;
}
icmphdr CreateIcmpHeader()
{
    struct icmphdr IcmpHeader;
    IcmpHeader.type = ICMP_ECHO;
    IcmpHeader.code=0;
    IcmpHeader.un.echo.id=1;//TODo/
    IcmpHeader.un.echo.sequence=2;//Todo
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
void ChangeTTL(int socket,int ttl)
{
    int TTL= ttl;
    if ( setsockopt(socket,IPPROTO_IP,IP_TTL,&TTL,sizeof(int)) < 0)
		ERROR ("Error while creating an socket");
}
void SendPacket(int socket,icmphdr IcmpHeader, sockaddr_in Rec)
{
    if (sendto(socket,&IcmpHeader,sizeof(IcmpHeader),0,(struct sockaddr*)&Rec,sizeof(Rec)) != sizeof(IcmpHeader))
		ERROR ("Sending error");
}
void Send (int ttl)
{
    int socfd = CreateSocket();
    icmphdr IcmpHeader = CreateIcmpHeader();
    sockaddr_in Rec = CreateSocketConnection();
    ChangeTTL(socfd,ttl);
    SendPacket(socfd,IcmpHeader,Rec);
}
void Validate(int numberOfArgs, char* ipAdress)
{
    if(numberOfArgs != 2)
        {
        printf("Format - sudo ./x.out <ip>\n");
        exit(1);
        }
    char * Split;
    Split = strtok(ipAdress,".");
    int PartsCounter=0;
    while (Split != NULL)
    {
    PartsCounter++;
    Split = strtok(NULL,".");
    }
    if(PartsCounter!=4)
        {
        printf("Wrong format - try x.x.x.x\n");
        exit(1);
        }
}
int main(int argc, char** argv)
{
    Validate(argc,argv[1]);
    for(int i=2;i<=5;i++)
    {
        Send(i);
    }

        return 0;
}

