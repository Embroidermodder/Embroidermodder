#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "emb-compress.h"
#include "emb-thread.h"

void husExpand(unsigned char* input, unsigned char* output, int compressedSize, int _269)
{
    currentPosition = 0;
    outputPosition = 0;
    currentIndex =0;
    inputBufferSize = bufferSize;
    mStatus = 0;
    outputArray = output;
    inputArray = input;
    inputSize = bufferSize;

    remainingBytes = compressedSize;
    if(_269 > _137 || _269 < _138)
    {
        mStatus = -1;
        _175 = 2;
    }
    else
    {
        _175 = (short) (1 << _269);
    }
    _172 = 0;
    _243 = 0;
    _246 = 0;
    _244 = 0;
    _245 = 0;

    _176 = (short) (_175 - 1);

    _166 = (unsigned char*)malloc(sizeof(unsigned char)*(_175+2));
    if(_166) memset(_166, 0, (_175+2)*sizeof(unsigned char));
    _240 = (unsigned short*)malloc(sizeof(unsigned short)*(_148));
    if(_240) memset(_240, 0, (_148)*sizeof(unsigned short));
    _241 = (unsigned short*)malloc(sizeof(unsigned short)*(_149));
    if(_241) memset(_241, 0, (_149)*sizeof(unsigned short));
    _189 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_141-1));
    if(_189) memset(_189, 0, (2*_141-1)*sizeof(unsigned short));
    _190 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_141-1));
    if(_190) memset(_190, 0, (2*_141-1)*sizeof(unsigned short));
    _180 = (unsigned char*)malloc(sizeof(unsigned char)*(_141));
    if(_180) memset(_180, 0, (_141)*sizeof(unsigned char));
    _181 = (unsigned char*)malloc(sizeof(unsigned char)*(_152));
    if(_181) memset(_181, 0, (_152)*sizeof(unsigned char));

    if( _166 == NULL ||
        _189 == NULL ||
        _190 == NULL ||
        _180 == NULL ||
        _181 == NULL ||
        _240 == NULL ||
        _241 == NULL ||
        inputBuffer == NULL)
    {
        mStatus = -1;
    }

    husExpand_expand();
    husExpand_cleanup();
}

void husExpand_cleanup(void)
{
    free(_166);
    free(_189);
    free(_190);
    free(_180);
    free(_181);
    free(_240);
    free(_241);
}

void husExpand_253(short _254,short _220,short _221)
{
    short _226,_203,_219;
    unsigned short _283;
    _219=husExpand_252(_220);
    if(_219==0)
    {
        _203=husExpand_252(_220);
        for(_226=0;_226<_254;_226++) _181[_226]=0;
        for(_226=0;_226<256;_226++) _241[_226]=_203;
    }
    else
    {
        _226=0;
        while(_226<_219)
        {
            _203=(short)(_182>>13);
            if(_203==7)
            {
                _283=1U<<12;
                while(_283&_182)
                {
                    _283>>=1;
                    _203++;
                }
            }
            husExpand_256((_203<7)?3:_203-3);
            _181[_226++]=(unsigned char )_203;
            if(_226==_221)
            {
                _203=husExpand_252(2);
                while(--_203>=0) _181[_226++]=0;
            }
        }
        while(_226<_254) _181[_226++]=0;
        husExpand_258(_254,_181,8,_241,_149);
    }
}

int husExpand_expand(void)
{
    short _200 = 0;
    unsigned char* _278 = _166;
    short _279 = _175;
    short _280 = _176;
    _243 = 0;
    husExpand_251();

    while(_243 < 5)
    {
        short _203;
        if((_203 = (short) husExpand_249()) <= byte_MAX)
        {
            _278[_200] = (unsigned char) _203;
            if(++_200 >= _279)
            {
                _200 = 0;
                memcpy(&outputArray[outputPosition], _278, _279);
                outputPosition += _279;
            }
        }
        else
        {
            short _226;
            short _276 = (short) (_203 - (byte_MAX + 1 - _135));
            if(_276 == _144) break;
            _226 = (short) ((_200 - husExpand_250() - 1) & _280);
            if(_226 < _279 - _140 - 1 && _200 < _279 - _140 - 1)
            {
                while(--_276 >= 0) _278[_200++] = _278[_226++];
            }
            else
            {
                while(--_276 >= 0)
                {
                    _278[_200] = _278[_226];
                    if(++_200 >= _279)
                    {
                        _200 = 0;
                        memcpy(&outputArray[outputPosition], _278, _279);
                        outputPosition += _279;
                    }
                    _226 = (short) ((_226 + 1) & _280);
                }
            }
        }
    }
    if(_200 != 0)
    {
        memcpy(&outputArray[outputPosition], _278, _200);
        outputPosition += _200;
    }
    return 0;
}

unsigned short husExpand_249(void)
{
    unsigned short _276,_283;
    if(_244==0)
    {
        _244=husExpand_252(16);
        husExpand_253(_145,_147,3);
        husExpand_255();
        husExpand_253(_142,_540,-1);
        if(mStatus<0) return 0;
    }
    _244--;
    _276=_240[_182>>4];
    if(_276>=_141)
    {
        _283=1U<<3;
        do
        {
            if(_182&_283) _276=_190[_276];
            else _276=_189[_276];
            _283>>=1;
        }
        while(_276 >= _141);
    }
    husExpand_256(_180[_276]);
    return _276;
}

unsigned short husExpand_250(void)
{
    unsigned short _276,_283;
    _276=_241[_182>>8];
    if(_276>=_142)
    {
        _283=1U<<7;
        do
        {
            if(_182&_283) _276=_190[_276];
            else _276=_189[_276];
            _283>>=1;
        }
        while(_276 >= _142);
    }
    husExpand_256(_181[_276]);
    if(_276!=0)
    {
        _276--;
        _276=(short)((1U<<_276)+husExpand_252(_276));
    }
    return _276;
}

void husExpand_251(void)
{
    _244 = 0;
    husExpand_257();
}

unsigned short husExpand_252(int _219)
{
    unsigned short _284 = (unsigned short) (_182 >> (16 - _219));
    husExpand_256(_219);
    return _284;
}

void husExpand_255(void)
{
    short _226, _203;
    short _219 = (short) husExpand_252(_143);
    if(_219 == 0)
    {
        _203 = (short) husExpand_252(_143);
        for(_226 = 0; _226 < _141; _226++) _180[_226] = 0;
        for(_226 = 0; _226 < _148; _226++) _240[_226] = (unsigned short) _203;
    }
    else
    {
        _226 = 0;
        while(_226 < _219)
        {
            _203 = (short) _241[_182 >> 8];
            if(_203 >= _145)
            {
                unsigned short _283 = (unsigned short) 1U << 7;
                do
                {
                    if((_182 & _283) != 0)
                    {
                        _203 = (short) _190[_203];
                    }
                    else
                    {
                        _203 = (short) _189[_203];
                    }
                    _283 >>= 1;
                }
                while(_203 >= _145);
            }
            husExpand_256(_181[_203]);
            if(_203 <= 2)
            {
                if(_203 == 0)
                {
                    _203 = 1;
                }
                else
                {
                    if(_203 == 1)
                    {
                        _203 = (short) (husExpand_252(4) + 3);
                    }
                    else
                    {
                        _203 = (short) (husExpand_252(_143) + 20);
                    }
                }
                while(--_203 >= 0)
                {
                    _180[_226++] = 0;
                }
            }
            else
            {
                _180[_226++] = (unsigned char) (_203 - 2);
            }
        }
        while(_226 < _141)
        {
            _180[_226++] = 0;
        }
        husExpand_258(_141, _180, 12, _240, _148);
    }
}

void husExpand_256(int _219)
{
    while(_219 > _172)
    {
        _219 -= _172;
        _182 = (unsigned short) ((_182 << _172) + (_245 >> (8 - _172)));
        if(_246 <= 0)
        {
            currentIndex = 0;

            if(remainingBytes >= 0 && remainingBytes < bufferSize)
            {
                inputBuffer = &inputArray[currentPosition];
                currentPosition += remainingBytes;
                _246 =  (short)remainingBytes;
                remainingBytes -= _246;
                inputBufferSize = _246;
            }
            else
            {
                inputBuffer = &inputArray[currentPosition];
                currentPosition += bufferSize;
                _246 = bufferSize;
                inputBufferSize = _246;
            }
            if(_246 <= 0) _243++;
        }
        _245 = inputBuffer[currentIndex++];
        _246--;
        _172 = 8;
    }
    _172 = (short) (_172 - _219);
    _182 = (unsigned short) ((_182 << _219) + (_245 >> (8 - _219)));
    _245 <<= _219;
}

void husExpand_257(void)
{
    _182 = 0;
    _245 = 0;
    _172 = 0;
    _246 = 0;
    husExpand_256(16);
}

void husExpand_258(int _259, unsigned char* _260, int _261, unsigned short* _262, unsigned short _263)
{
    unsigned short _277[17],_287[17],_288[18],*_204;
    unsigned int _226,_289,_209,_290,_291,_292,_293,_283;
    _288[0] = 0;
    _277[0] = 0;
    _287[0] = 0;

    for(_226=1;_226<=16;_226++) _277[_226]=0;
    for(_226=0;(int)_226<_259;_226++) _277[_260[_226]]++;
    _288[1]=0;
    for(_226=1;_226<=16;_226++) _288[_226+1]=(unsigned short)(_288[_226]+(_277[_226]<<(16-_226)));
    if(_288[17] != (unsigned short)(1U<<16))
    {
        mStatus = -1;
        _243=10;
        return;
    }
    _291=16-_261;
    for(_226=1;(int )_226<=_261;_226++)
    {
        _288[_226]>>=_291;
        _287[_226]=(unsigned short )(1U<<(_261-_226));
    }
    while(_226<=16)
    {
        _287[_226]=(unsigned short )(1U<<(16-_226));
        _226++;
    }
    _226=_288[_261+1]>>_291;
    if(_226!=(unsigned short )(1U<<16))
    {
        _289=1U<<_261;
        while(_226!=_289) _262[_226++]=0;
    }
    _292=_259;
    _283=1U<<(15-_261);
    for(_290=0;(int)_290<_259;_290++)
    {
        if((_209=_260[_290])==0) continue;
        _293=_288[_209]+_287[_209];
        if((int)_209<=_261)
        {
            if(_293 > _263)
            {
                mStatus = -1;
                _243 = 10;
                return;
            }
            for(_226=_288[_209];_226<_293;_226++) _262[_226]=(unsigned short )_290;
        }
        else
        {
            _289=_288[_209];
            _204=&_262[_289>>_291];
            _226=_209-_261;
            while(_226!=0)
            {
                if(*_204==0)
                {
                    _190[_292]=_189[_292]=0;
                    *_204=(unsigned short )_292++;
                }
                if(_289&_283) _204=&_190[*_204];
                else _204=&_189[*_204];
                _289<<=1;
                _226--;
            }
            *_204=(unsigned short )_290;
        }
        _288[_209]=(unsigned short )_293;
    }
}

/*****************************************
* HUS Compress Functions
****************************************/

int husCompress(unsigned char* _266, unsigned long _inputSize, unsigned char* _267, int _269, int _235)
{
    int returnVal;
    inputArray = _266;
    outputArray = _267;
    _531 = _235;
    if(_269 > _137 || _269 < _138)
    {
        mStatus = -1;
        _175 = 2;
    }
    else
    {
        _175 = (short)(1<<_269);
    }
    _176 = (short)(_175-1);

    _166 = (unsigned char*)malloc(sizeof(unsigned char)*(_175+_140+2));
    if(_166) memset(_166, 0, (_175+_140+2)*sizeof(unsigned char));
    _163 = (short*)malloc(sizeof(short)*(_175+_153));
    if(_163) memset(_163, 0, (_175+_153)*sizeof(short));
    _164 = (short*)malloc(sizeof(short)*(_175));
    if(_164) memset(_164, 0, (_175)*sizeof(short));
    _165 = (unsigned char*)malloc(sizeof(unsigned char)*(_155));
    if(_165) memset(_165, 0, (_155)*sizeof(unsigned char));
    _179 = (unsigned char*)malloc(sizeof(unsigned char)*(_156));
    if(_179) memset(_179, 0, (_156)*sizeof(unsigned char));
    _189 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_141-1));
    if(_189) memset(_189, 0, (2*_141-1)*sizeof(unsigned short));
    _190 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_141-1));
    if(_190) memset(_190, 0, (2*_141-1)*sizeof(unsigned short));
    _177 = (short*)malloc(sizeof(short)*(_141+1));
    if(_177) memset(_177, 0, (_141+1)*sizeof(short));
    _180 = (unsigned char*)malloc(sizeof(unsigned char)*(_141));
    if(_180) memset(_180, 0, (_141)*sizeof(unsigned char));
    _191 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_141-1));
    if(_191) memset(_191, 0, (2*_141-1)*sizeof(unsigned short));
    _192 = (unsigned short*)malloc(sizeof(unsigned short)*(_141));
    if(_192) memset(_192, 0, (_141)*sizeof(unsigned short));
    _181 = (unsigned char*)malloc(sizeof(unsigned char)*(_152));
    if(_181) memset(_181, 0, (_152)*sizeof(unsigned char));
    _193 = (unsigned short*)malloc(sizeof(unsigned short)*(2*_142-1));
    if(_193) memset(_193, 0, (2*_142-1)*sizeof(unsigned short));
    _194 = (unsigned short*)malloc(sizeof(unsigned short)*(_152));
    if(_194) memset(_194, 0, (_152)*sizeof(unsigned short));

    if(!_166|| !_163|| !_164|| !_165|| !_179|| !_189|| !_190|| !_177|| !_180|| !_191|| !_192|| !_181|| !_193|| !_194)
    {
        mStatus = -1;
    }
    _533 = 0;
    _534 = _inputSize;
    inputLength = _inputSize;
    inputPosition = 0;
    outputPosition = 0;

    returnVal = husCompress_compress();
    husCompress_cleanup();
    return returnVal;
}

void husCompress_cleanup(void)
{
    free(_166);
    free(_163);
    free(_164);
    free(_165);
    free(_179);
    free(_189);
    free(_190);
    free(_177);
    free(_180);
    free(_191);
    free(_192);
    free(_181);
    free(_193);
    free(_194);
}

void husCompress_223(short _203)
{
    husCompress_208(_180[_203], _192[_203]);
}

int husCompress_compress(void)
{
    short _209;
    short _201;
    short _200;
    short s;
    int _231;
    unsigned char* _278;
    short _280;
    short _279;
    _278 = _166;
    _280 = _176;
    _279 = _175;
    _231 = 0;
    husCompress_196();
    husCompress_198();
    _200 = 0;

    memcpy(_278, &inputArray[inputPosition], _279);

    inputPosition += _279;
    if(inputPosition > inputLength)
    {
        _209 = (short)(inputLength - inputPosition);
    }
    else
    {
        _209 = _279;
    }
    s = (short)(_209&_280);
    _169 = 0;
    _168 = 0;
    _201 = (short)(((_278[_200]<<_154)^(_278[_200+1]))&(_153-1));
    _201 = (short)(husCompress_445(_200,_201)+_279);
    while(_209 > _140 + 4 && !_170)
    {
        husCompress_199(_200, _201);
        if(_168 < _135)
        {
            husCompress_202(_278[_200], 0);
            husCompress_447(_200, _201);
            _200++;
            _201 = (short)(husCompress_445(_200,_201)+_279);
            _209--;
        }
        else
        {
            _209 -= _168;
            husCompress_202((unsigned short)(_168+(UCHAR_MAX+1-_135)), _169);
            while(--_168 >= 0)
            {
                husCompress_447(_200, _201);
                _200++;
                _201 = (short)(husCompress_445(_200, _201) + _279);
            }
        }
    }
    for(; _209 < _140; _209++)
    {
        int _203;
        if(inputPosition >= inputLength) 
            break;
        _203 = (int)(unsigned char)inputArray[inputPosition];
        inputPosition += 1;
        _278[s] = (unsigned char)_203;
        if(s < _140 - 1) 
            _278[s + _279] = _278[s];
        husCompress_448(s);
        s = (short)((s + 1)&(_280));
    }
    while(_209 > 0 && !_170)
    {
        husCompress_199(_200, _201);
        if(_168 > _209)
            _168 = _209;
        if(_168 < _135)
        {
            _168 = 1;
            husCompress_202(_278[_200], 0);
        }
        else
            husCompress_202((unsigned short)(_168+(UCHAR_MAX + 1 - _135)), _169);
        while(--_168 >= 0)
        {
            int _203;
            if(inputPosition >= inputLength) 
                break;
            _203 = (int)(unsigned char) inputArray[inputPosition];
            inputPosition += 1;
            _278[s] = (unsigned char)_203;
            if(s < _140 - 1)
                _278[s+_279] = _278[s];
            husCompress_448(s);
            s = (short)((s + 1)&(_280));
            husCompress_447(_200, _201);
            _200 = (short)((_200 + 1)&(_280));
            _201 = (short)(husCompress_445(_200, _201) + _279);
        }
        while(_168-- >= 0)
        {
            husCompress_447(_200, _201);
            _200 = (short)((_200 + 1)&_280);
            _201 = (short)(husCompress_445(_200, _201) + _279);
            _209--;
        }
        if(mStatus < 0)
            return 1;
    }
    if(!_170)
        husCompress_202(_144+(UCHAR_MAX + 1 - _135), 0);
    husCompress_197();
    return outputPosition;
}

void husCompress_196(void)
{
    int i;
    for(i = 0; i < _141; i++)
        _191[i] = 0;
    for(i = 0; i < _142; i++)
        _193[i] = 0;
    _173 = 0;
    husCompress_205();
    _170 = 0;
    _185 = 1;
    _184 = 0;
    _186 = 0;
    _165[0] = 0;
    _183 = _155;
    _183 -= (unsigned short)((3*CHAR_BIT)+6);
}

void husCompress_197(void)
{
    if(!_170)
        husCompress_207();
    husCompress_206();
    _183 = 0;
    _184 = 0;
}

void husCompress_198(void)
{
    int i;
    short* _450;
    _450 = &_163[_175];

    for(i = _153; i > 0; i--)
        *_450++ = _157;
    _450 = _164;
    for(i = _175; i > 0; i--)
        *_450++ = _157;
}

void husCompress_199(short _200, short _201)
{
    unsigned char* _451;
    unsigned char* _278;
    short _226, _452, _204, _453;
    _452 = _158;
    _168 = 0;
    _451 = &_166[_200];
    _204 = _201;
    while((_204 = _163[_204]) != _157)
    {
        if(--_452 < 0)
            break;
        _278 = &_166[_204];
        if(_451[_168] != _278[_168])
            continue;
        if(_451[0] != _278[0])
            continue;
        if(_451[1] != _278[1])
            continue;
        if(_451[2] != _278[2])
            continue;
        for(_226 = 3; _226 < _140; _226++)
            if(_451[_226]!=_278[_226])
                break;
        if(_226 > _168)
        {
            _453 = (short)(_200 - _204 - 1);
            if(_453 < 0)
                _453 += _175;
            if(_453 >= _175)
                break;
            _169 = _453;
            if((_168 = _226) >= _140)
                break;
        }
    }
}

void husCompress_202(unsigned short _203, unsigned short _204)
{
    if((_185>>=1) == 0)
    {
        _185=1U<<(CHAR_BIT-1);
        if(_184 >= _183)
        {
            husCompress_207();
            if(_170)
                return;
            _184 = 0;
        }
        _186 = _184++;
        _165[_186] = 0;
    }
    _165[_184++] = (unsigned char)_203;
    _191[_203]++;
    if(_203 >= (1U<<CHAR_BIT))
    {
        _165[_186] |= (unsigned char)_185;
        _165[_184++] = (unsigned char)_204;
        _165[_184++] = (unsigned char)(_204>>CHAR_BIT);
        _203 = 0;
        while(_204)
        {
            _203++;
            _204>>=1;
        }
        _193[_203]++;
    }
}

void husCompress_205(void)
{
    _172 = 0;
    _182 = 0;
    _171 = 0;
}

void husCompress_206(void)
{
    if(!_170)
    {
        husCompress_208(CHAR_BIT-1, 0);
        if(_171)
        {
            husCompress_210();
        }
    }
    _171 = 0;
}

void husCompress_207(void)
{
    unsigned int _226, _289, _229, _454, _455;
    unsigned int _456 = 0;
    unsigned short _217[2 * _145 - 1];
    _229 = husCompress_211(_141, _191, _180, _192);
    _455 = _191[_229];
    husCompress_208(16, (unsigned short)_455);
    if(_229 >= _141)
    {
        husCompress_216(_217);
        _229 = husCompress_211(_145, _217, _181, _194);
        if(_229 >= _145)
        {
            husCompress_218(_145, _147, 3);
        }
        else
        {
            husCompress_208(_147, 0);
            husCompress_208(_147, (unsigned short)_229);
        }
        husCompress_222();
    }
    else
    {
        husCompress_208(_147, 0);
        husCompress_208(_147, 0);
        husCompress_208(_143, 0);
        husCompress_208(_143, (unsigned short)_229);
    }
    _229 = husCompress_211(_142, _193, _181, _194);
    if(_229 >= _142)
    {
        husCompress_218(_142, _540, -1);
    }
    else
    {
        husCompress_208(_540, 0);
        husCompress_208(_540, (unsigned short )_229);
    }
    _454 = 0;
    for(_226 = 0; _226 < _455; _226++)
    {
        if(_226 % CHAR_BIT == 0)
            _456 = _165[_454++];
        else
            _456<<=1;
        if(_456&(1U<<(CHAR_BIT-1)))
        {
            husCompress_223((short)(_165[_454++]+(1U<<CHAR_BIT)));
            _289 = _165[_454++];
            _289 += _165[_454++]<<CHAR_BIT;
            husCompress_224((short)_289);
        }
        else
            husCompress_223(_165[_454++]);
        if(_170)
            return;
    }
    for(_226 = 0; _226 < _141; _226++)
        _191[_226] = 0;
    for(_226 = 0; _226 < _142; _226++)
        _193[_226] = 0;
}

void husCompress_208(int _209, unsigned short _203)
{
    _203<<=_133-_209;
    _182|=(unsigned short)(_203>>_172);
    if((_172 += (short)_209) >= 8)
    {
        if(_171 >= _156)
            husCompress_210();
        _179[_171++] = (unsigned char)(_182>>CHAR_BIT);
        if((_172 = (unsigned short)(_172- CHAR_BIT))<CHAR_BIT)
            _182<<=CHAR_BIT;
        else
        {
            if(_171 >= _156)
                husCompress_210();
            _179[_171++] = (unsigned char)_182;
            _172 = (unsigned short)(_172-CHAR_BIT);
            _182 = (unsigned short)(_203<<(_209-_172));
        }
    }
}

void husCompress_210(void)
{
    if(_171 <= 0)
        return;
    if(_531 && (_533 += _171) >= _534)
        _170 = 1;
    else
    {
        memcpy(outputArray + outputPosition, _179, _171);
        outputPosition += _171;
    }
    _171 = 0;
}

int husCompress_211(int _212, unsigned short* _213, unsigned char* _214, unsigned short* _215)
{
    int _226, _276, _289, _292;
    short _227;
    _174 = (short)_212;
    _187 = _213;
    _178 = _214;
    _292 = _174;
    _227 = 0;
    _177[1] = 0;
    for(_226 = 0; _226 < _174; _226++)
    {
        _178[_226] = 0;
        if(_187[_226])
        {
            _177[++_227] = (short)_226;
        }
    }
    if(_227 < 2)
    {
        _215[_177[1]]=0;
        return _177[1];
    }
    for(_226 = _227/2; _226 >= 1; _226--)
    {
        husCompress_225(_226, _187, _177, _227);
    }
    _188 = _215;
    do
    {
        _226 = _177[1];
        if(_226 < _174)
        {
            *_188++=(unsigned short)_226;
        }
        _177[1] = _177[_227--];
        husCompress_225(1, _187, _177, _227);
        _276 = _177[1];
        if(_276 < _174)
            *_188++ = (unsigned short)_276;
        _289 = _292++;
        _187[_289] = (unsigned short)(_187[_226] + _187[_276]);
        _177[1] = (short)_289;
        husCompress_225(1, _187, _177, _227);
        _189[_289] = (unsigned short)_226;
        _190[_289] = (unsigned short)_276;
    }
    while(_227 > 1);
    _188 = _215;
    husCompress_228(_289);
    husCompress_230(_212, _214, _215);
    return _289;
}

void husCompress_216(unsigned short* _217)
{
    short _226, _289, _219, _277;
    for(_226 = 0; _226 < _145; _226++)
        _217[_226] = 0;
    _219 = _141;
    while(_219 > 0 && _180[_219-1] == 0)
        _219--;
    _226 = 0;
    while(_226 < _219)
    {
        _289 = _180[_226++];
        if(_289 == 0)
        {
            _277 = 1;
            while(_226 < _219 && _180[_226] == 0)
            {
                _226++;
                _277++;
            }
            if(_277 <= 2) 
                _217[0] += _277;
            else if(_277 <= 18)
                _217[1]++;
            else if(_277 == 19)
            {
                _217[0]++;
                _217[1]++;
            }
            else
                _217[2]++;
        }
        else
        {
            _217[_289+2]++;
        }
    }
}

void husCompress_218(short _219, short _220, short _221)
{
    short _226, _289;
    while(_219 > 0 && _181[_219-1] == 0)
        _219--;
    husCompress_208(_220, _219);
    _226 = 0;
    while(_226 < _219)
    {
        _289 = _181[_226++];
        if(_289 <= 6)
        {
            husCompress_208(3, _289);
        }
        else
            husCompress_208(_289-3, (unsigned short)(USHRT_MAX<<1));
        if(_226 == _221)
        {
            while(_226 < 6 && _181[_226] == 0)
                _226++;
            husCompress_208(2, (unsigned short)(_226-3));
        }
    }
}

void husCompress_222(void)
{
    short _226, _289, _219, _277;
    _219 = _141;
    while(_219 > 0 && _180[_219-1] == 0)
        _219--;
    husCompress_208(_143, _219);
    _226 = 0;
    while(_226 < _219)
    {
        _289 = _180[_226++];
        if(_289 == 0)
        {
            _277 = 1;
            while(_226 < _219 && _180[_226] == 0)
            {
                _226++;
                _277++;
            }
            if(_277 <= 2)
            {
                for(_289 = 0; _289 < _277; _289++)
                    husCompress_208(_181[0], _194[0]);
            }
            else if(_277 <= 18)
            {
                husCompress_208(_181[1], _194[1]);
                husCompress_208(4, (unsigned short)(_277-3));
            }
            else if(_277 == 19)
            {
                husCompress_208(_181[0], _194[0]);
                husCompress_208(_181[1], _194[1]);
                husCompress_208(4, 15);
            }
            else
            {
                husCompress_208(_181[2], _194[2]);
                husCompress_208(_143, (unsigned short)(_277-20));
            }
        }
        else
            husCompress_208(_181[_289+2], _194[_289+2]);
    }
}

void husCompress_224(unsigned short _204)
{
    unsigned short _203, _457;
    _203 = 0;
    _457 = _204;
    while(_457)
    {
        _203++;
        _457>>=1;
    }
    husCompress_208(_181[_203], _194[_203]);
    if(_203 > 1)
        husCompress_208(_203-1, _204);
}

void husCompress_225(int _226, unsigned short* _187, short* _177, short _227)
{
    int _276, _289;
    _289 = _177[_226];
    while((_276 = 2 * _226) <= _227)
    {
        if(_276 < _227 && _187[_177[_276]] > _187[_177[_276+1]])
            _276++;
        if(_187[_289] <= _187[_177[_276]])
            break;
        _177[_226] = _177[_276];
        _226 = _276;
    }
    _177[_226] = (unsigned short)_289;
}

void husCompress_228(int _229)
{
    int _226, _289;
    unsigned int _458;
    for(_226 = 0; _226 <= 16; _226++)
        _167[_226] = 0;
    husCompress_232(_229);
    _458 = 0;
    for(_226 = 16; _226 > 0; _226--)
        _458+=_167[_226]<<(16-_226);
    while(_458 != (1U<<16))
    {
        _167[16]--;
        for(_226 = 15; _226 > 0; _226--)
        {
            if(_167[_226] != 0)
            {
                _167[_226]--;
                _167[_226+1] = (unsigned short)(_167[_226+1]+2);
                break;
            }
        }
        _458--;
    }
    for(_226 = 16; _226 > 0; _226--)
    {
        _289 = _167[_226];
        while(--_289 >= 0)
            _178[*_188++] = (unsigned char)_226;
    }
}

void husCompress_230(int _219, unsigned char* _209, unsigned short* _231)
{
    int _226;
    unsigned short _288[18];
    _288[1] = 0;
    for(_226 = 1; _226 <= 16; _226++)
        _288[_226+1] = (unsigned short)((_288[_226]+_167[_226])<<1);
    for(_226 = 0; _226 < _219; _226++)
        _231[_226] = _288[_209[_226]]++;
}

void husCompress_232(int _226)
{
    if(_226 < _174)
        _167[(_173<16)?_173:16]++;
    else
    {
        _173++;
        husCompress_232(_189[_226]);
        husCompress_232(_190[_226]);
        _173--;
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
