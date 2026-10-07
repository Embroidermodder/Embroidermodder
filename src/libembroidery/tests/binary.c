/*
 * Tests of binary functions.
 */

#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "tests.h"

const char *test_file_data = "TEST DATA 01";

int createBinaryTestFile1(void)
{
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    embFile_write(test_file_data, 1, 12, file);
    embFile_close(file);
    return 1;
}

/* Similar to the supplied htobe16, etc. in "endian.h" */

int16_t btoi16(char *b)
{
    return b[0] + 0x100 * b[1];
}

int16_t btoi16be(char *b)
{
    return 0x100 * b[0] + b[1];
}

int32_t btoi32(char *b)
{
    return 0x1000000 * b[3] + 0x10000 * b[2] + 0x100 * b[1] + b[0];
}

int32_t btoi32be(char *b)
{
    return 0x1000000 * b[0] + 0x10000 * b[1] + 0x100 * b[2] + b[3];
}

int read_byte_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadByte(file) != test_file_data[0]) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_bytes_test(void)
{
    int result = 1;
    if (!createBinaryTestFile1()) {
        return 0;
    }
    unsigned char dest[20];
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (!binaryReadBytes(file, dest, 12)) {
        result = 0;
    }
    else {
        if (strncmp(test_file_data, dest, 12)) {
            result = 0;
        }
    }
    embFile_close(file);
    return result;
}

int read_int16_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt16(file) != btoi16(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_int32_test(void)
{
    int result = 1;
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt32(file) != btoi32(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_uint8_test(void)
{
    int result = 1;
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt8(file) != 'T') {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_uint16_test(void)
{
    int result = 1;
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt16(file) != btoi16(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_uint32_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt32(file) != btoi32(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_int16be_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt16BE(file) != btoi16be(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_uint16be_test(void)
{
    int result = 1;
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt16BE(file) != btoi16be(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_int32be_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt32BE(file) != btoi32be(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_uint32be_test(void)
{
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt32BE(file) != btoi32be(test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_string_test(void)
{
    char buffer[100];
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    binaryReadString(file, buffer, strlen(test_file_data));
    if (strcmp(buffer, test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_ustring_test(void)
{
    char buffer[100];
    if (!createBinaryTestFile1()) {
        return 0;
    }
    EmbFile *file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    binaryReadUnicodeString(file, buffer, strlen(test_file_data));
    if (strcmp(buffer, test_file_data)) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int read_float_test(void)
{
    float data = 3.141592;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        embFile_close(file);
        return 0;
    }
    binaryWriteFloat(file, data);
    embFile_close(file);
    
    file = embFile_open("test.dat", "rb");
    if (fabs(binaryReadFloat(file) - data) > 0.00000001) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_byte_test(void)
{
    uint8_t byte = 15;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteByte(file, byte);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadByte(file) != byte) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_bytes_test(void)
{
    /*
    binaryWriteBytes(file, data, size);
    */
    return 0;
}

int write_int16_test(void)
{
    int16_t data = 4328;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteShort(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt16(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_int16be_test(void)
{
    int16_t data = 4328;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteShortBE(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt16BE(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_uint16_test(void)
{
    uint16_t data = 4328;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteUShort(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt16(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_uint16be_test(void)
{
    uint16_t data = 4328;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteUShortBE(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt16BE(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_int32_test(void)
{
    int32_t data = 432832;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteInt(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt32(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_int32be_test(void)
{
    int32_t data = 432832;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteIntBE(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadInt32BE(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_uint32_test(void)
{
    uint32_t data = 432832;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteUInt(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt32(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_uint32be_test(void)
{
    uint32_t data = 432832;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        return 0;
    }
    binaryWriteUIntBE(file, data);
    embFile_close(file);

    file = embFile_open("test.dat", "rb");
    if (!file) {
        return 0;
    }
    if (binaryReadUInt32BE(file) != data) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

int write_float_test(void)
{
    float data = 3.141592;
    EmbFile *file = embFile_open("test.dat", "wb");
    if (!file) {
        embFile_close(file);
        return 0;
    }
    binaryWriteFloat(file, data);
    embFile_close(file);
    
    file = embFile_open("test.dat", "rb");
    if (fabs(binaryReadFloat(file) - data) > 0.00000001) {
        embFile_close(file);
        return 0;
    }
    embFile_close(file);
    return 1;
}

