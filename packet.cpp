//packet.cpp
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include "helpers.h"
#include "socket.h"

unsigned char* BufferPointer;
timeval TimersIn[30][3];
double TimeDifferences[30][3];
char* IPadresses[30][3];
void ShiftBytes(int count)
{
    BufferPointer += count;
}

int Pid =getpid();

icmphdr CreateIcmpHeader(int ttl, int pid)
{
    struct icmphdr IcmpHeader;
    IcmpHeader.type = ICMP_ECHO;
    IcmpHeader.code=0;
    IcmpHeader.un.echo.id=pid;
    IcmpHeader.un.echo.sequence=ttl;
    IcmpHeader.checksum=0;
    IcmpHeader.checksum = ComputeIcmpChecksum((u_int16_t*)&IcmpHeader,sizeof(IcmpHeader));
    return IcmpHeader;
}



void Send (int ttl,int socfd,int pid,char* ip)
{
    for (int i=0; i<3; i++)
    {
        int checksum = ttl*10+i;
        icmphdr IcmpHeader = CreateIcmpHeader(checksum,pid);
        sockaddr_in Rec = CreateSocketConnection(ip);
        ChangeTTL(socfd,ttl);
        SendPacket(socfd,IcmpHeader,Rec);
        timeval SendingTime;
        gettimeofday(&SendingTime,NULL);
        TimersIn[ttl][i]= SendingTime;
    }
}
void Print(int TTL)
{

    char ret[150];
    strcpy(ret,IPadresses[TTL][0]);
    strcat(ret," ");
    for(int i=1; i<3; i++)
    {
        char* ptr;
        ptr = strstr(ret,IPadresses[TTL][i]);
        if(ptr == NULL)
        {
            strcat(ret,IPadresses[TTL][i]);
            strcat(ret," ");
        }
    }

    double avg=0;
    bool flag=true;
    for(int i=0; i<3; i++)
    {
        if(TimeDifferences[TTL][i]<0)
        {
            flag=false;
            break;
        }
        avg+=TimeDifferences[TTL][i];
    }
    avg/=3;
    if(flag)
        printf ("%d. %s : %fms \n", TTL,ret,avg);
    else
        printf ("%d. %s : ??? \n", TTL,ret);
}
void Process(icmp* IcmpPacket,sockaddr_in sender,int i)
{
    int TTL = IcmpPacket->icmp_seq/10;
    int Sequence = IcmpPacket->icmp_seq%10;
    char sender_ip_str[20];
    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    timeval tim;
    gettimeofday(&tim,NULL);
    double elapsedTime = (tim.tv_sec - TimersIn[TTL][Sequence].tv_sec) * 1000.0;
    elapsedTime += (tim.tv_usec - TimersIn[TTL][Sequence].tv_usec) / 1000.0;
    TimeDifferences[TTL][Sequence] = elapsedTime;
    IPadresses[TTL][Sequence]= sender_ip_str;
    if(i==3)
        Print(TTL);
}

bool Recieve(int sockfd,int ttl)
{
    int i=0;
    int notResponding=0;
    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(sockfd,&descriptors);
    struct sockaddr_in 	sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET+1];

    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    while(1)
    {
        if(notResponding==3)
        {
            printf("%d. * \n",ttl);
            break;
        }
        if (i==3)
        {
            break;
        }
        int ready = Select(sockfd+1, &descriptors, NULL, NULL, &timeout);
        if(ready)
        {

            ssize_t packet_len = Recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, &sender, &sender_len);
            if (packet_len < 0)
            {
                continue;
            }
            BufferPointer = buffer;
            ip* IpPacket = (ip*) BufferPointer;
            ShiftBytes(IpPacket->ip_hl*4);
            icmp* IcmpPacket = (icmp*) BufferPointer;
            ShiftBytes(8);
            if((IcmpPacket->icmp_type == ICMP_TIME_EXCEEDED &&
                    IcmpPacket->icmp_code == ICMP_EXC_TTL) )
            {
                ShiftBytes(IpPacket->ip_hl*4);
                IcmpPacket = (icmp*)BufferPointer;
                if (IcmpPacket->icmp_id == Pid && IcmpPacket->icmp_seq/10 == ttl)
                {
                    i++;
                    Process(IcmpPacket,sender,i);
                }
            }

            else if(IcmpPacket->icmp_type == ICMP_ECHOREPLY && IcmpPacket->icmp_id == Pid&& IcmpPacket->icmp_seq/10 == ttl)
            {
                i++;
                Process(IcmpPacket,sender,i);
                if(i==3)
                    return false;
            }
        }
        else
        {
            notResponding++;
            i++;
            TimeDifferences[ttl][0]=TimeDifferences[ttl][1]=TimeDifferences[ttl][2]=-1;
        }
    }
    return true;
}
