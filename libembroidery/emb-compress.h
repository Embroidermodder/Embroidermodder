/*! @file emb-compress.h */
#ifndef EMB_COMPRESS_H
#define EMB_COMPRESS_H

extern void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
extern int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);

/*****************************************
 * HUS Expand/Compress Constants
 ****************************************/
#define _132 (CHAR_BIT*sizeof(unsigned short))
#define _133 (16)
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
extern short* _163;
extern short* _164;
extern unsigned char* _165;
extern unsigned char* _166;
extern unsigned short _167[17];
extern short _168;
extern short _169;
extern short _170;
extern short _171;
extern short _172;
extern short _173;
extern short _174;
extern short _175;
extern short _176;
extern short* _177;
extern unsigned char* _178;
extern unsigned char* _179;
extern unsigned char* _180;
extern unsigned char* _181;
extern unsigned short _182;
extern unsigned short _183;
extern unsigned short _184;
extern unsigned short _185;
extern unsigned short _186;
extern unsigned short* _187;
extern unsigned short* _188;
extern unsigned short* _189;
extern unsigned short* _190;
extern unsigned short* _191;
extern unsigned short* _192;
extern unsigned short* _193;
extern unsigned short* _194;
extern unsigned short* _240;
extern unsigned short* _241;
extern short _243;
extern unsigned short _244;
extern unsigned char _245;
extern short _246;
extern int _531;
extern unsigned long _533;
extern unsigned long _534;

extern int mStatus;
extern int currentIndex;
extern long remainingBytes;
extern int inputSize;
extern int inputBufferSize;
extern int inputLength;
extern unsigned char* outputArray;
extern unsigned char* inputArray;
extern unsigned char* inputBuffer;
extern int currentPosition;
extern int inputPosition;
extern int outputPosition;

/*****************************************
 * HUS Expand Functions
 ****************************************/
extern void husExpand_258(int _259, unsigned char* _260, int _261, unsigned short* _262, unsigned short _263);
extern void husExpand_257(void);
extern void husExpand_256(int _219);
extern void husExpand_255(void);
extern void husExpand_253(short _254, short _220, short _221);
extern unsigned short husExpand_252(int _219);
extern void husExpand_251(void);
extern unsigned short husExpand_250(void);
extern unsigned short husExpand_249(void);

extern void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269);
extern int husExpand_expand(void);
extern void husExpand_cleanup(void);

/*****************************************
 * HUS Compress Functions
 ****************************************/
extern void husCompress_196(void);
extern void husCompress_197(void);
extern void husCompress_198(void);
extern void husCompress_199(short _200, short _201);
extern void husCompress_202(unsigned short _203, unsigned short _204);
extern void husCompress_205(void);
extern void husCompress_206(void);
extern void husCompress_207(void);
extern void husCompress_208(int _209, unsigned short _203);
extern void husCompress_210(void);
extern int husCompress_211(int _212, unsigned short* _213, unsigned char* _214, unsigned short* _215);
extern void husCompress_216(unsigned short* _217);
extern void husCompress_218(short _219, short _220, short _221);
extern void husCompress_222(void);
extern void husCompress_223(short _203);
extern void husCompress_224(unsigned short _204);
extern void husCompress_225(int _226, unsigned short* _187, short* _177, short _227);
extern void husCompress_228(int _229);
extern void husCompress_230(int _219, unsigned char* _209, unsigned short* _231);
extern void husCompress_232(int _226);

/* extern int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235);
extern int husCompress(unsigned char* _233, unsigned long _inputSize, unsigned char* _202, int _234, int _235);*/
extern void husCompress_cleanup(void);
extern int husCompress_compress(void);

/*TODO: macros are nasty, bleh */
#define husCompress_445(_200,_446)((short)((_446<<_154)^(_278[_200+2]))&(_153-1))
#define husCompress_447(_200,_201){short _204;if((_204=_163[_201])!=_157)_164[_204]=_200;_164[_200]=_201;_163[_200]=_204;_163[_201]=_200;}
#define husCompress_448(s){short _204;if((_204=_164[s])!=_157){_164[s]=_157;_163[_204]=_157;}}

#endif /* EMB_COMPRESS_H */
