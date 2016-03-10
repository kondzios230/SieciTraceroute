//socket.h
#include <netinet/ip_icmp.h>

int CreateSocket();
void SendPacket(int socket,icmphdr IcmpHeader, sockaddr_in Rec);
void ChangeTTL(int socket,int ttl);
