#ifndef FORMAT_SVG_H
#define FORMAT_SVG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

typedef struct SvgAttribute_     SvgAttribute;
typedef struct SvgAttributeList_ SvgAttributeList;
typedef struct SvgElement_       SvgElement;

struct SvgAttribute_
{
    char* name;
    char* value;
};

struct SvgAttributeList_
{
    SvgAttribute attribute;
    SvgAttributeList* next;
};

struct SvgElement_
{
    char* name;
    SvgAttributeList* attributeList;
    SvgAttributeList* lastAttribute;
};

typedef enum
{
    SVG_EXPECT_NULL,
    SVG_EXPECT_ELEMENT,
    SVG_EXPECT_ATTRIBUTE,
    SVG_EXPECT_VALUE
} SVG_EXPECT;

typedef enum
{
    SVG_NULL,
    SVG_ELEMENT,
    SVG_PROPERTY,
    SVG_MEDIA_PROPERTY,
    SVG_ATTRIBUTE,
    SVG_CATCH_ALL
} SVG_TYPES;

int readSvg(EmbPattern* pattern, const char* fileName);
int writeSvg(EmbPattern* pattern, const char* fileName);

int svgExpect;

SvgElement* currentElement;
char* currentAttribute;
char* currentValue;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_SVG_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
