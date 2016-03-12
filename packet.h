#include <arpa/inet.h>
#include <unistd.h>

icmphdr CreateIcmpHeader(int orderNumber,int pid);
sockaddr_in CreateSocketConnection(char* ip);
void Send (int ttl,int socfd,int pid,char* ip);
void Recieve(int sockfd,int ttl);
