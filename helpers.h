//helpers.h
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
u_int16_t ComputeIcmpChecksum (const u_int16_t *buff, int length);
void Validate(int numberOfArgs, char* ipAdress);
void ShiftBytes(int count);
void PrintAsBytes (unsigned char* buff, ssize_t length);
