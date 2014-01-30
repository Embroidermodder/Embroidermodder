/*! @file format-svg.h */
#ifndef FORMAT_SVG_H
#define FORMAT_SVG_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

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
    SVG_CREATOR_NULL,
    SVG_CREATOR_EMBROIDERMODDER,
    SVG_CREATOR_ILLUSTRATOR,
    SVG_CREATOR_INKSCAPE
} SVG_CREATOR;

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

extern EMB_PRIVATE int EMB_CALL readSvg(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeSvg(EmbPattern* pattern, const char* fileName);

int svgCreator;

int svgExpect;
int svgMultiValue;

SvgElement* currentElement;
char* currentAttribute;
char* currentValue;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_SVG_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
