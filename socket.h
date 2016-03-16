//socket.h
#include <netinet/ip_icmp.h>

int CreateSocket();
void SendPacket(int socket,icmphdr IcmpHeader, sockaddr_in Rec);
void ChangeTTL(int socket,int ttl);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

ssize_t Recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr);
