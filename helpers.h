//helpers.h
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
u_int16_t compute_icmp_checksum (const u_int16_t *buff, int length);
void Validate(int numberOfArgs, char* ipAdress);
