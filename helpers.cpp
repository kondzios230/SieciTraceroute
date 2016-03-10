//helpers.cpp
#include "helpers.h"

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
