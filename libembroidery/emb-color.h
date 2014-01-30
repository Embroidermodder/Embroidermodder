/*! @file emb-color.h */
#ifndef EMB_COLOR_H
#define EMB_COLOR_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbColor_
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EmbColor;

extern EMB_PUBLIC EmbColor EMB_CALL embColor_make(unsigned char r, unsigned char g, unsigned char b);
extern EMB_PUBLIC EmbColor* EMB_CALL embColor_create(unsigned char r, unsigned char g, unsigned char b);
extern EMB_PUBLIC EmbColor EMB_CALL embColor_fromHexStr(char* val);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_COLOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
