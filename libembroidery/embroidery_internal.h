#ifndef EMBROIDERY_INTERNAL
#define EMBROIDERY_INTERNAL

#include "embroidery.h"

#define PI 3.1415926535

#define EMB_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define EMB_MIN(a,b) (((a) < (b)) ? (a) : (b))

/**
 * @brief 
 * 
 */
typedef struct Huffman {
    int default_value;
    int lengths[1000];
    int nlengths;
    int table[1000];
    int table_width;
    int ntable;
} huffman;

/**
 * @brief 
 * 
 */
typedef struct Compress {
    int bit_position;
    char *input_data;
    int input_length;
    int bits_total;
    int block_elements;
    huffman character_length_huffman;
    huffman character_huffman;
    huffman distance_huffman;
} compress;

int read100(EmbPattern* pattern, const char* fileName);
int write100(EmbPattern* pattern, const char* fileName);
int read10o(EmbPattern* pattern, const char* fileName);
int write10o(EmbPattern* pattern, const char* fileName);
int readArt(EmbPattern* pattern, const char* fileName);
int writeArt(EmbPattern* pattern, const char* fileName);
int readBmc(EmbPattern* pattern, const char* fileName);
int writeBmc(EmbPattern* pattern, const char* fileName);
int readBro(EmbPattern* pattern, const char* fileName);
int writeBro(EmbPattern* pattern, const char* fileName);
int readCnd(EmbPattern* pattern, const char* fileName);
int writeCnd(EmbPattern* pattern, const char* fileName);
int readCol(EmbPattern* pattern, const char* fileName);
int writeCol(EmbPattern* pattern, const char* fileName);
int readCsd(EmbPattern* pattern, const char* fileName);
int writeCsd(EmbPattern* pattern, const char* fileName);
int readCsv(EmbPattern* pattern, const char* fileName);
int writeCsv(EmbPattern* pattern, const char* fileName);
int readDat(EmbPattern* pattern, const char* fileName);
int writeDat(EmbPattern* pattern, const char* fileName);
int readDem(EmbPattern* pattern, const char* fileName);
int writeDem(EmbPattern* pattern, const char* fileName);
int readDsb(EmbPattern* pattern, const char* fileName);
int writeDsb(EmbPattern* pattern, const char* fileName);
int readDst(EmbPattern* pattern, const char* fileName);
int writeDst(EmbPattern* pattern, const char* fileName);
int readDsz(EmbPattern* pattern, const char* fileName);
int writeDsz(EmbPattern* pattern, const char* fileName);
int readDxf(EmbPattern* pattern, const char* fileName);
int writeDxf(EmbPattern* pattern, const char* fileName);
int readEdr(EmbPattern* pattern, const char* fileName);
int writeEdr(EmbPattern* pattern, const char* fileName);
int readEmd(EmbPattern* pattern, const char* fileName);
int writeEmd(EmbPattern* pattern, const char* fileName);
int readExp(EmbPattern* pattern, const char* fileName);
int writeExp(EmbPattern* pattern, const char* fileName);
int readExy(EmbPattern* pattern, const char* fileName);
int writeExy(EmbPattern* pattern, const char* fileName);
int readEys(EmbPattern* pattern, const char* fileName);
int writeEys(EmbPattern* pattern, const char* fileName);
int readFxy(EmbPattern* pattern, const char* fileName);
int writeFxy(EmbPattern* pattern, const char* fileName);
int readGc(EmbPattern* pattern, const char* fileName);
int writeGc(EmbPattern* pattern, const char* fileName);
int readGnc(EmbPattern* pattern, const char* fileName);
int writeGnc(EmbPattern* pattern, const char* fileName);
int readGt(EmbPattern* pattern, const char* fileName);
int writeGt(EmbPattern* pattern, const char* fileName);
int readHus(EmbPattern* pattern, const char* fileName);
int writeHus(EmbPattern* pattern, const char* fileName);
int readInb(EmbPattern* pattern, const char* fileName);
int writeInb(EmbPattern* pattern, const char* fileName);
int readJef(EmbPattern* pattern, const char* fileName);
int writeJef(EmbPattern* pattern, const char* fileName);
int readInb(EmbPattern* pattern, const char* fileName);
int writeInb(EmbPattern* pattern, const char* fileName);
int readInf(EmbPattern* pattern, const char* fileName);
int writeInf(EmbPattern* pattern, const char* fileName);
int readKsm(EmbPattern* pattern, const char* fileName);
int writeKsm(EmbPattern* pattern, const char* fileName);
int readMax(EmbPattern* pattern, const char* fileName);
int writeMax(EmbPattern* pattern, const char* fileName);
int readPlt(EmbPattern* pattern, const char* fileName);
int writePlt(EmbPattern* pattern, const char* fileName);
int readRgb(EmbPattern* pattern, const char* fileName);
int writeRgb(EmbPattern* pattern, const char* fileName);
int readPhc(EmbPattern* pattern, const char* fileName);
int writePhc(EmbPattern* pattern, const char* fileName);
int readPhb(EmbPattern* pattern, const char* fileName);
int writePhb(EmbPattern* pattern, const char* fileName);
int readPes(EmbPattern* pattern, const char* fileName);
int writePes(EmbPattern* pattern, const char* fileName);
int readPem(EmbPattern* pattern, const char* fileName);
int writePem(EmbPattern* pattern, const char* fileName);
int readPel(EmbPattern* pattern, const char* fileName);
int writePel(EmbPattern* pattern, const char* fileName);
int readPec(EmbPattern* pattern, const char* fileName);
int writePec(EmbPattern* pattern, const char* fileName);
void readPecStitches(EmbPattern* pattern, EmbFile* file);
void writePecStitches(EmbPattern* pattern, EmbFile* file, const char* filename);
int readMit(EmbPattern* pattern, const char* fileName);
int writeMit(EmbPattern* pattern, const char* fileName);
int readNew(EmbPattern* pattern, const char* fileName);
int writeNew(EmbPattern* pattern, const char* fileName);
int readOfm(EmbPattern* pattern, const char* fileName);
int writeOfm(EmbPattern* pattern, const char* fileName);
int readPcd(EmbPattern* pattern, const char* fileName);
int writePcd(EmbPattern* pattern, const char* fileName);
int readPcm(EmbPattern* pattern, const char* fileName);
int writePcm(EmbPattern* pattern, const char* fileName);
int readPcq(EmbPattern* pattern, const char* fileName);
int writePcq(EmbPattern* pattern, const char* fileName);
int readPcs(EmbPattern* pattern, const char* fileName);
int writePcs(EmbPattern* pattern, const char* fileName);
int readSew(EmbPattern* pattern, const char* fileName);
int writeSew(EmbPattern* pattern, const char* fileName);
int readShv(EmbPattern* pattern, const char* fileName);
int writeShv(EmbPattern* pattern, const char* fileName);
int readSvg(EmbPattern* pattern, const char* fileName);
int writeSvg(EmbPattern* pattern, const char* fileName);
int readThr(EmbPattern* pattern, const char* fileName);
int writeThr(EmbPattern* pattern, const char* fileName);
int readT01(EmbPattern* pattern, const char* fileName);
int writeT01(EmbPattern* pattern, const char* fileName);
int readT09(EmbPattern* pattern, const char* fileName);
int writeT09(EmbPattern* pattern, const char* fileName);
int readTap(EmbPattern* pattern, const char* fileName);
int writeTap(EmbPattern* pattern, const char* fileName);
int readSst(EmbPattern* pattern, const char* fileName);
int writeSst(EmbPattern* pattern, const char* fileName);
int readStx(EmbPattern* pattern, const char* fileName);
int writeStx(EmbPattern* pattern, const char* fileName);
int readTxt(EmbPattern* pattern, const char* fileName);
int writeTxt(EmbPattern* pattern, const char* fileName);
int readU00(EmbPattern* pattern, const char* fileName);
int writeU00(EmbPattern* pattern, const char* fileName);
int readU01(EmbPattern* pattern, const char* fileName);
int writeU01(EmbPattern* pattern, const char* fileName);
int readVip(EmbPattern* pattern, const char* fileName);
int writeVip(EmbPattern* pattern, const char* fileName);
int readVp3(EmbPattern* pattern, const char* fileName);
int writeVp3(EmbPattern* pattern, const char* fileName);
int readXxx(EmbPattern* pattern, const char* fileName);
int writeXxx(EmbPattern* pattern, const char* fileName);
int readZsk(EmbPattern* pattern, const char* fileName);
int writeZsk(EmbPattern* pattern, const char* fileName);

extern void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
extern int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);

#include "hashtable.h"

typedef HashTable EmbHash;

EMB_PUBLIC EmbHash* embHash_create(void);
EMB_PUBLIC void embHash_free(EmbHash* hash);

EMB_PUBLIC int embHash_contains(const EmbHash* hash, const void* key);
EMB_PUBLIC int embHash_insert(EmbHash* hash, const void* key, void* value);
EMB_PUBLIC void* embHash_value(const EmbHash* hash, const void* key);
EMB_PUBLIC void embHash_remove(EmbHash* hash, const void *key);
EMB_PUBLIC void embHash_clear(EmbHash* hash);
EMB_PUBLIC int embHash_empty(const EmbHash* hash);
EMB_PUBLIC long embHash_count(const EmbHash* hash);
EMB_PUBLIC void embHash_rehash(EmbHash* hash, long numOfBuckets);

#endif
