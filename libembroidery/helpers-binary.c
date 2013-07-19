#include "helpers-binary.h"

char binaryReadByte(FILE* file)
{
    return fgetc(file);
}

void binaryReadBytes(FILE* file, unsigned char* destination, int count)
{
    fread((char*) destination, count, 1, file);
}

int binaryReadInt16(FILE* file)
{
    short x = fgetc(file);
    x = x | fgetc(file) << 8;
    return x;
}

int binaryReadInt32(FILE* file)
{
    int x = fgetc(file);
    x = x | fgetc(file) << 8;
    x = x | fgetc(file) << 16;
    x = x | fgetc(file) << 24;
    return x;
}

unsigned char binaryReadUInt8(FILE* file)
{
    return (unsigned char)fgetc(file);
}

unsigned short binaryReadUInt16(FILE* file)
{
    unsigned short x = fgetc(file);
    x = x | fgetc(file) << 8;
    return x;
}

unsigned int binaryReadUInt32(FILE* file)
{
    unsigned int x = fgetc(file);
    x = x | fgetc(file) << 8;
    x = x | fgetc(file) << 16;
    x = x | fgetc(file) << 24;
    return x;
}

/* Big endian version */
short binaryReadInt16BE(FILE* file)
{
    short returnValue = fgetc(file) << 8;
    returnValue |= fgetc(file);
    return returnValue;
}

/* Big endian version */
unsigned short binaryReadUInt16BE(FILE* file)
{
    unsigned short returnValue = fgetc(file) << 8;
    returnValue |= fgetc(file);
    return returnValue;
}

/* Big endian version */
int binaryReadInt32BE(FILE* file)
{
    int returnValue = fgetc(file) << 24;
    returnValue |= fgetc(file) << 16;
    returnValue |= fgetc(file) << 8;
    returnValue |= fgetc(file);
    return (returnValue);
}

float binaryReadFloat(FILE* file)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.u32 = fgetc(file);
    float_int_u.u32 |= fgetc(file) << 8;
    float_int_u.u32 |= fgetc(file) << 16;
    float_int_u.u32 |= fgetc(file) << 24;
    return float_int_u.f32;
}

void binaryWriteByte(FILE* file, unsigned char data)
{
    fputc(data, file);
}

void binaryWriteBytes(FILE* file, const char* data, int size)
{
    fwrite((char*)data, 1, size, file);
}

void binaryWriteShort(FILE* file, short data)
{
    fputc(data & 0xFF, file);
    fputc((data >> 8) & 0xFF, file);
}

void binaryWriteUShort(FILE* file, unsigned short data)
{
    fputc(data & 0xFF, file);
    fputc((data >> 8) & 0xFF, file);
}

void binaryWriteInt(FILE* file, int data)
{
    fputc(data & 0xFF, file);
    fputc((data >> 8) & 0xFF, file);
    fputc((data >> 16) & 0xFF, file);
    fputc((data >> 24) & 0xFF, file);
}

void binaryWriteUInt(FILE* file, unsigned int data)
{
    fputc(data & 0xFF, file);
    fputc((data >> 8) & 0xFF, file);
    fputc((data >> 16) & 0xFF, file);
    fputc((data >> 24) & 0xFF, file);
}

void binaryWriteFloat(FILE* file, float data)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.f32 = data;

    fputc(float_int_u.u32 & 0xFF, file);
    fputc((float_int_u.u32 >> 8) & 0xFF, file);
    fputc((float_int_u.u32 >> 16) & 0xFF, file);
    fputc((float_int_u.u32 >> 24) & 0xFF, file);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
