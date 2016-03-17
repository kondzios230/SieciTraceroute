//helpers.cpp
#include "helpers.h"

u_int16_t ComputeIcmpChecksum (const u_int16_t *buff, int length)
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
        printf("Format - sudo ./TracerouteSieci <ip>\n");
        exit(1);
    }

}

void PrintAsBytes(unsigned char* buff, ssize_t length)
{
    for (ssize_t i = 0; i < length; i++, buff++)
        printf ("%.2x ", *buff);
}
