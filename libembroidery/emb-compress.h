#ifndef EMB_COMPRESS_H
#define EMB_COMPRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);

/*****************************************
 * HUS Expand/Compress Constants
 ****************************************/
#define _132 (CHAR_BIT*sizeof(unsigned short))
#define _133 (16)
/*TODO: Why is _134 unused? */
#define _134 ('\0')
#define _135 (3)
#define _136 (16384)
#define _137 (14)
#define _138 (10)
#define _139 (8)
#define _140 (256)
#define byte_MAX (255)
#define _141 (byte_MAX + 1 + _140 - _135 + 1 + 1)
#define _142 (_137 + 1)
#define _143 (9)
#define _144 (_140 + 1)
#define _145 (_133 + 3)
#define _147 (5)
#define _148 (4096)
#define _149 (256)

#define _152 (_145)
#define _153 (4096)
#define _154 (4)
#define _155 (8192)
#define _156 (512)
#define _157 (-1)
#define _158 (128)
#define _159 (512)
#define _540 (5)
#define bufferSize (512)

/*****************************************
 * HUS Expand/Compress Variables
 ****************************************/
short* _163;
short* _164;
unsigned char* _165;
unsigned char* _166;
unsigned short _167[17];
short _168;
short _169;
short _170;
short _171;
short _172;
short _173;
short _174;
short _175;
short _176;
short* _177;
unsigned char* _178;
unsigned char* _179;
unsigned char* _180;
unsigned char* _181;
unsigned short _182;
unsigned short _183;
unsigned short _184;
unsigned short _185;
unsigned short _186;
unsigned short* _187;
unsigned short* _188;
unsigned short* _189;
unsigned short* _190;
unsigned short* _191;
unsigned short* _192;
unsigned short* _193;
unsigned short* _194;
unsigned short* _240;
unsigned short* _241;
short _243;
unsigned short _244;
unsigned char _245;
short _246;
int _531;
unsigned long _533;
unsigned long _534;

int mStatus;
int currentIndex;
long remainingBytes;
int inputSize;
int inputBufferSize;
int inputLength;
unsigned char* outputArray;
unsigned char* inputArray;
unsigned char* inputBuffer;
int currentPosition;
int inputPosition;
int outputPosition;

/*****************************************
 * HUS Expand Functions
 ****************************************/
void husExpand_258(int _259, unsigned char* _260, int _261, unsigned short* _262, unsigned short _263);
void husExpand_257();
void husExpand_256(int _219);
void husExpand_255();
void husExpand_253(short _254, short _220, short _221);
unsigned short husExpand_252(int _219);
void husExpand_251();
unsigned short husExpand_250();
unsigned short husExpand_249();

void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
int husExpand_expand();
void husExpand_cleanup();

/*****************************************
 * HUS Compress Functions
 ****************************************/
void husCompress_196();
void husCompress_197();
void husCompress_198();
void husCompress_199(short _200, short _201);
void husCompress_202(unsigned short _203, unsigned short _204);
void husCompress_205();
void husCompress_206();
void husCompress_207();
void husCompress_208(int _209, unsigned short _203);
void husCompress_210();
int husCompress_211(int _212, unsigned short* _213, unsigned char* _214, unsigned short* _215);
void husCompress_216(unsigned short* _217);
void husCompress_218(short _219, short _220, short _221);
void husCompress_222();
void husCompress_223(short _203);
void husCompress_224(unsigned short _204);
void husCompress_225(int _226, unsigned short* _187, short* _177, short _227);
void husCompress_228(int _229);
void husCompress_230(int _219, unsigned char* _209, unsigned short* _231);
void husCompress_232(int _226);

/* int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);
int husCompress(unsigned char* _233, unsigned long _inputSize, unsigned char* _202, int _234, int _235);*/
void husCompress_cleanup();
int husCompress_compress();

/*TODO: macros are nasty, bleh */
#define husCompress_445(_200,_446)((short)((_446<<_154)^(_278[_200+2]))&(_153-1))
#define husCompress_447(_200,_201){short _204;if((_204=_163[_201])!=_157)_164[_204]=_200;_164[_200]=_201;_163[_200]=_204;_163[_201]=_200;}
#define husCompress_448(s){short _204;if((_204=_164[s])!=_157){_164[s]=_157;_163[_204]=_157;}}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_COMPRESS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
