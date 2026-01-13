#include "helpers-binary.h"

char binaryReadByte(EmbFile* file)
{
    return (char)embFile_getc(file);
}

int binaryReadBytes(EmbFile* file, unsigned char* destination, int count)
{
    return (int) embFile_read((char*) destination, 1, count, file);
}

short binaryReadInt16(EmbFile* file)
{
    int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    return (short)x;
}

int binaryReadInt32(EmbFile* file)
{
    int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    x = x | embFile_getc(file) << 16;
    x = x | embFile_getc(file) << 24;
    return x;
}

unsigned char binaryReadUInt8(EmbFile* file)
{
    return (unsigned char)embFile_getc(file);
}

unsigned short binaryReadUInt16(EmbFile* file)
{
    return (unsigned short)(embFile_getc(file) | embFile_getc(file) << 8);
}

unsigned int binaryReadUInt32(EmbFile* file)
{
    unsigned int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    x = x | embFile_getc(file) << 16;
    x = x | embFile_getc(file) << 24;
    return x;
}

/* Big endian version */
short binaryReadInt16BE(EmbFile* file)
{
    short returnValue = (short)(embFile_getc(file) << 8);
    returnValue |= embFile_getc(file);
    return returnValue;
}

/* Big endian version */
unsigned short binaryReadUInt16BE(EmbFile* file)
{
    unsigned short returnValue = (unsigned short)(embFile_getc(file) << 8);
    returnValue |= embFile_getc(file);
    return returnValue;
}

/* Big endian version */
int binaryReadInt32BE(EmbFile* file)
{
    int returnValue = embFile_getc(file) << 24;
    returnValue |= embFile_getc(file) << 16;
    returnValue |= embFile_getc(file) << 8;
    returnValue |= embFile_getc(file);
    return (returnValue);
}

/* Big endian version */
unsigned int binaryReadUInt32BE(EmbFile* file)
{
    unsigned int returnValue = embFile_getc(file) << 24;
    returnValue |= embFile_getc(file) << 16;
    returnValue |= embFile_getc(file) << 8;
    returnValue |= embFile_getc(file);
    return returnValue;
}

void binaryReadString(EmbFile* file, char* buffer, int maxLength)
{
    int i = 0;
    while(i < maxLength)
    {
        buffer[i] = (char)embFile_getc(file);
        if(buffer[i] == '\0') break;
        i++;
    }
}

void binaryReadUnicodeString(EmbFile* file, char *buffer, const int stringLength)
{
    int i = 0;
    for(i = 0; i < stringLength * 2; i++)
    {
        char input = (char)embFile_getc(file);
        if(input != 0)
        {
            buffer[i] = input;
        }
    }
}

float binaryReadFloat(EmbFile* file)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.u32 = embFile_getc(file);
    float_int_u.u32 |= embFile_getc(file) << 8;
    float_int_u.u32 |= embFile_getc(file) << 16;
    float_int_u.u32 |= embFile_getc(file) << 24;
    return float_int_u.f32;
}

void binaryWriteByte(EmbFile* file, unsigned char data)
{
    embFile_putc(data, file);
}

void binaryWriteBytes(EmbFile* file, const char* data, int size)
{
    embFile_write((char*)data, 1, size, file);
}

void binaryWriteShort(EmbFile* file, short data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
}

void binaryWriteShortBE(EmbFile* file, short data)
{
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteUShort(EmbFile* file, unsigned short data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
}

void binaryWriteUShortBE(EmbFile* file, unsigned short data)
{
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteInt(EmbFile* file, int data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 24) & 0xFF, file);
}

void binaryWriteIntBE(EmbFile* file, int data)
{
    embFile_putc((data >> 24) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteUInt(EmbFile* file, unsigned int data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 24) & 0xFF, file);
}

void binaryWriteUIntBE(EmbFile* file, unsigned int data)
{
    embFile_putc((data >> 24) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteFloat(EmbFile* file, float data)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.f32 = data;

    embFile_putc(float_int_u.u32 & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 8) & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 16) & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 24) & 0xFF, file);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
