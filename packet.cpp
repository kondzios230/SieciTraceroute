//packet.cpp
#include <arpa/inet.h>
#include <unistd.h>

#include "helpers.h"
#include "socket.h"

unsigned char* BufferPointer;

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
sockaddr_in CreateSocketConnection(char * ip)
{
    struct sockaddr_in Rec;
    bzero(&Rec, sizeof(Rec));
    Rec.sin_family=AF_INET;
    inet_pton(AF_INET,ip, &Rec.sin_addr);
    return Rec;
}


void Send (int ttl,int socfd,int pid,char* ip)
{
    for (int i=0;i<3;i++)
    {
        icmphdr IcmpHeader = CreateIcmpHeader(ttl,pid);
        sockaddr_in Rec = CreateSocketConnection(ip);
        ChangeTTL(socfd,ttl);
        SendPacket(socfd,IcmpHeader,Rec);
    }
}

void Recieve(int sockfd,int ttl)
{
 int i=0;
        fd_set descriptors;
        FD_ZERO(&descriptors);
		FD_SET(sockfd,&descriptors);
		struct sockaddr_in 	sender;
		socklen_t sender_len = sizeof(sender);
		u_int8_t buffer[IP_MAXPACKET+1];
        timeval timeout; timeout.tv_sec = 1; timeout.tv_usec = 0;
		while(1)
		{
            int ready = Select(sockfd+1, &descriptors, NULL, NULL, &timeout);
			if(ready)
			{
                if (i==3){break;}
                ssize_t packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
                if (packet_len < 0) {continue;}
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
                    if (IcmpPacket->icmp_id == Pid && IcmpPacket->icmp_seq == ttl)
                    {
                        char sender_ip_str[20];
                        inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
                        printf ("Received IP packet with ICMP content from: %s\n", sender_ip_str);
                        i++;
                    }
                }

                else if(IcmpPacket->icmp_type == ICMP_ECHOREPLY)
				{
                    ShiftBytes(IpPacket->ip_hl*4);
                    IcmpPacket = (icmp*)BufferPointer;
                    if (IcmpPacket->icmp_id == Pid )
                    {
                        char sender_ip_str[20];
                        inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
                        printf ("Received IP packet with ICMP content from: %s\n", sender_ip_str);
                        i++;
                    }

				}
            }
            else
            {
                if(i!=3)
                {printf("%d - \n",ttl);}
                break;
            }
        }
}
