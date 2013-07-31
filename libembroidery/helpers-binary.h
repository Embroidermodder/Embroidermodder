#ifndef HELPERS_BINARY_H
#define HELPERS_BINARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

char binaryReadByte(FILE* file);
int binaryReadBytes(FILE* file, unsigned char* destination, int count);
int binaryReadInt16(FILE* file);
int binaryReadInt32(FILE* file);
unsigned char binaryReadUInt8(FILE* file);
unsigned short binaryReadUInt16(FILE* file);
unsigned int binaryReadUInt32(FILE* file);
short binaryReadInt16BE(FILE* file); /* Big endian version */
unsigned short binaryReadUInt16BE(FILE* file); /* Big endian version */
int binaryReadInt32BE(FILE* file);   /* Big endian version */
unsigned int binaryReadUInt32BE(FILE* file);
float binaryReadFloat(FILE* file);
void binaryReadString(FILE* file, char *buffer, int maxLength);

void binaryWriteByte(FILE* file, unsigned char data);
void binaryWriteBytes(FILE* file, const char* data, int size);
void binaryWriteShort(FILE* file, short data);
void binaryWriteUShort(FILE* file, unsigned short data);
void binaryWriteUShortBE(FILE* file, unsigned short data);
void binaryWriteInt(FILE* file, int data);
void binaryWriteUInt(FILE* file, unsigned int data);
void binaryWriteUIntBE(FILE* file, unsigned int data);
void binaryWriteFloat(FILE* file, float data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HELPERS_BINARY_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
