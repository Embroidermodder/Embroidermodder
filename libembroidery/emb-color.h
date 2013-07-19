#ifndef EMB_COLOR_H
#define EMB_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbColor_
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

EmbColor embColor_fromHexStr(char* val);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_COLOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
