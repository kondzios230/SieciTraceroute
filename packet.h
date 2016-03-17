#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>

icmphdr CreateIcmpHeader(int orderNumber,int pid);
sockaddr_in CreateSocketConnection(char* ip);
void Send (int ttl,int socfd,int pid,char* ip);
bool Recieve(int sockfd,int ttl);
