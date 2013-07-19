#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "format-svg.h"
#include "helpers-misc.h"

SvgAttribute svgAttribute_create(const char* name, const char* value)
{
    SvgAttribute attribute;
    char* modValue = emb_strdup(value);
    int last = strlen(modValue);
    int i = 0;
    for(i = 0; i < last; i++)
    {
        if(modValue[i] == '"') modValue[i] = ' ';
        if(modValue[i] == '/') modValue[i] = ' ';
        if(modValue[i] == ',') modValue[i] = ' ';
    }
    attribute.name = emb_strdup(name);
    attribute.value = modValue;
    return attribute;
}

void svgElement_addAttribute(SvgElement* element, SvgAttribute data)
{
    if(!element) { /* TODO: error */ return; }

    if(!(element->attributeList))
    {
        element->attributeList = (SvgAttributeList*)malloc(sizeof(SvgAttributeList));
        if(!(element->attributeList)) { /* TODO: error */ return; }
        element->attributeList->attribute = data;
        element->attributeList->next = 0;
        element->lastAttribute = element->attributeList;
        element->lastAttribute->next = 0;
    }
    else
    {
        SvgAttributeList* pointerLast = element->lastAttribute;
        SvgAttributeList* list = (SvgAttributeList*)malloc(sizeof(SvgAttributeList));
        if(!list) { /* TODO: error */ return; }
        list->attribute = data;
        list->next = 0;
        pointerLast->next = list;
        element->lastAttribute = list;
    }
}

void svgElement_free(SvgElement* element)
{
    SvgAttributeList* list;
    SvgAttributeList* nextList = 0;
    if(!element) return;

    list = element->attributeList;

    while(list)
    {
        free(list->attribute.name);
        list->attribute.name = 0;
        free(list->attribute.value);
        list->attribute.value = 0;
        nextList = list->next;
        free(list);
        list = 0;
        list = nextList;
    }

    element->lastAttribute = 0;
    free(element->name);
    free(element);
}

SvgElement* svgElement_create(const char* name)
{
    SvgElement* element = 0;
    element = (SvgElement*)malloc(sizeof(SvgElement));
    if(!element) { /* TODO: error */ return 0; }
    element->name = emb_strdup(name);
    if(!element->name) { /* TODO: error */ return 0; }
    element->attributeList = 0;
    element->lastAttribute = 0;
    return element;
}

char* svgAttribute_getValue(SvgElement* element, const char* name)
{
    SvgAttributeList* pointer;
    if(!element) { /* TODO: error */ return "none"; }
    if(!element->attributeList) { /* TODO: error */ return "none"; }

    pointer = element->attributeList;
    while(pointer)
    {
        if(!strcmp(pointer->attribute.name, name)) { return pointer->attribute.value; }
        pointer = pointer->next;
    }

    return "none";
}

void svgAddToPattern(EmbPattern* p)
{
    const char* buff;
    if(!currentElement) { return; }

    buff = currentElement->name;
    if(!buff) { return; }

    if     (!strcmp(buff, "?xml"))             {  }
    else if(!strcmp(buff, "a"))                {  }
    else if(!strcmp(buff, "animate"))          {  }
    else if(!strcmp(buff, "animateColor"))     {  }
    else if(!strcmp(buff, "animateMotion"))    {  }
    else if(!strcmp(buff, "animateTransform")) {  }
    else if(!strcmp(buff, "animation"))        {  }
    else if(!strcmp(buff, "audio"))            {  }
    else if(!strcmp(buff, "circle"))
    {
        embPattern_addCircleObjectAbs(p, atof(svgAttribute_getValue(currentElement, "cx")),
                                        atof(svgAttribute_getValue(currentElement, "cy")),
                                        atof(svgAttribute_getValue(currentElement, "r")));
    }
    else if(!strcmp(buff, "defs"))             {  }
    else if(!strcmp(buff, "desc"))             {  }
    else if(!strcmp(buff, "discard"))          {  }
    else if(!strcmp(buff, "ellipse"))
    {
        embPattern_addEllipseObjectAbs(p, atof(svgAttribute_getValue(currentElement, "cx")),
                                        atof(svgAttribute_getValue(currentElement, "cy")),
                                        atof(svgAttribute_getValue(currentElement, "rx")),
                                        atof(svgAttribute_getValue(currentElement, "ry")));
    }
    else if(!strcmp(buff, "font"))             {  }
    else if(!strcmp(buff, "font-face"))        {  }
    else if(!strcmp(buff, "font-face-src"))    {  }
    else if(!strcmp(buff, "font-face-uri"))    {  }
    else if(!strcmp(buff, "foreignObject"))    {  }
    else if(!strcmp(buff, "g"))                {  }
    else if(!strcmp(buff, "glyph"))            {  }
    else if(!strcmp(buff, "handler"))          {  }
    else if(!strcmp(buff, "hkern"))            {  }
    else if(!strcmp(buff, "image"))            {  }
    else if(!strcmp(buff, "line"))
    {
        char* x1 = svgAttribute_getValue(currentElement, "x1");
        char* y1 = svgAttribute_getValue(currentElement, "y1");
        char* x2 = svgAttribute_getValue(currentElement, "x2");
        char* y2 = svgAttribute_getValue(currentElement, "y2");

        /* If the starting and ending points are the same, it is a point */
        if(!strcmp(x1, x2) && !strcmp(y1, y2))
            embPattern_addPointObjectAbs(p, atof(x1), atof(y1));
        else
            embPattern_addLineObjectAbs(p, atof(x1), atof(y1), atof(x2), atof(y2));
    }
    else if(!strcmp(buff, "linearGradient"))   {  }
    else if(!strcmp(buff, "listener"))         {  }
    else if(!strcmp(buff, "metadata"))         {  }
    else if(!strcmp(buff, "missing-glyph"))    {  }
    else if(!strcmp(buff, "mpath"))            {  }
    else if(!strcmp(buff, "path"))
    {
        /* TODO: finish */
    }
    else if(!strcmp(buff, "polygon"))          {  }
    else if(!strcmp(buff, "polyline"))         {  }
    else if(!strcmp(buff, "prefetch"))         {  }
    else if(!strcmp(buff, "radialGradient"))   {  }
    else if(!strcmp(buff, "rect"))
    {
        embPattern_addRectObjectAbs(p, atof(svgAttribute_getValue(currentElement, "x")),
                                    atof(svgAttribute_getValue(currentElement, "y")),
                                    atof(svgAttribute_getValue(currentElement, "width")),
                                    atof(svgAttribute_getValue(currentElement, "height")));
    }
    else if(!strcmp(buff, "script"))           {  }
    else if(!strcmp(buff, "set"))              {  }
    else if(!strcmp(buff, "solidColor"))       {  }
    else if(!strcmp(buff, "stop"))             {  }
    else if(!strcmp(buff, "svg"))              {  }
    else if(!strcmp(buff, "switch"))           {  }
    else if(!strcmp(buff, "tbreak"))           {  }
    else if(!strcmp(buff, "text"))             {  }
    else if(!strcmp(buff, "textArea"))         {  }
    else if(!strcmp(buff, "title"))            {  }
    else if(!strcmp(buff, "tspan"))            {  }
    else if(!strcmp(buff, "use"))              {  }
    else if(!strcmp(buff, "video"))            {  }

    svgElement_free(currentElement);
}


int svgIsElement(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "?xml"))             { type = SVG_NULL; } /* TODO: Fix the xml version ? messing with svg version */
    else if(!strcmp(buff, "a"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animate"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateColor"))     { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateMotion"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateTransform")) { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animation"))        { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "audio"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "circle"))           { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "defs"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "desc"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "discard"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "ellipse"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font-face"))        { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font-face-src"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font-face-uri"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "foreignObject"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "g"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "glyph"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "handler"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "hkern"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "image"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "line"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "linearGradient"))   { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "listener"))         { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "metadata"))         { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "missing-glyph"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "mpath"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "path"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "polygon"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "polyline"))         { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "prefetch"))         { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "radialGradient"))   { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "rect"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "script"))           { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "set"))              { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "solidColor"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "stop"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "svg"))              { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "switch"))           { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "tbreak"))           { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "text"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "textArea"))         { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "title"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "tspan"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "use"))              { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "video"))            { type = SVG_ELEMENT; }
    else                                       { type = SVG_NULL; }
    return type;
}

int svgIsMediaProperty(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "audio-level"))           { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "buffered-rendering"))    { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "display"))               { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "image-rendering"))       { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "pointer-events"))        { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "shape-rendering"))       { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "text-rendering"))        { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill"))         { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill-opacity")) { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "visibility"))            { type = SVG_MEDIA_PROPERTY; }
    return type;
}

int svgIsProperty(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "audio-level"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "buffered-rendering"))    { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "color"))                 { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "color-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "direction"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "display"))               { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "display-align"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill"))                  { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill-opacity"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill-rule"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-family"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-size"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-style"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-variant"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-weight"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "image-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "line-increment"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "opacity"))               { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "pointer-events"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "shape-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "solid-color"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "solid-opacity"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stop-color"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stop-opacity"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke"))                { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-dasharray"))      { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-linecap"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-linejoin"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-miterlimit"))     { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-opacity"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-width"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-align"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-anchor"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-rendering"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "unicode-bidi"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "vector-effect"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill-opacity")) { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "visibility"))            { type = SVG_PROPERTY; }
    return type;
}

int svgIsXmlAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "encoding"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "standalone")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "version"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))          { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsLinkAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "target"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAnimateAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAnimateColorAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAnimateMotionAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyPoints"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "origin"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "path"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rotate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAnimateTransformAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAnimationAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsAudioAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsCircleAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "r"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsDefsAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsDescAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsDiscardAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsEllipseAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ry"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsFontAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-origin-x"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsFontFaceAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accent-height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "alphabetic"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ascent"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "bbox"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cap-height"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "descent"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-stretch"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "hanging"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ideographic"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mathematical"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overline-position"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overline-thickness"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "panose-1"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "slope"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "stemh"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "stemv"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "strikethrough-position"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "strikethrough-thickness"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "underline-position"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "underline-thickness"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "unicode-range"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "units-per-em"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "widths"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x-height"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsFontFaceSrcAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsFontFaceUriAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsForeignObjectAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsGroupAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsGlyphAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "arabic-form")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "glyph-name"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "lang"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "unicode"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))           { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsHandlerAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ev:event"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsHKernAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "g1"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "g2"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "k"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "u1"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "u2"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsImageAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "opacity"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsLineAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x1"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x2"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y1"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y2"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsLinearGradientAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "gradientUnits")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x1"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x2"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y1"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y2"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsListenerAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "defaultAction")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "event"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "handler"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "observer"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "phase"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "propagate"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "target"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsMetadataAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsMissingGlyphAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))           { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsMPathAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsPathAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "pathLength"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsPolygonAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "points"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsPolylineAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "points"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsPrefetchAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "bandwidth"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaCharacterEncoding")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaContentEncodings"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaSize"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaTime"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                      { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsRadialGradientAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "gradientUnits")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "r"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsRectAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ry"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsScriptAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsSetAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsSolidColorAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsStopAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "offset"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsSvgAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "baseProfile"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "contentScriptType"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "playbackOrder"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "snapshotTime"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehaviorDefault"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncToleranceDefault"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "timelineBegin"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "version"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "viewBox"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "zoomAndPan"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsSwitchAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsTBreakAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsTextAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "editable"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rotate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsTextAreaAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "editable"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsTitleAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsTSpanAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsUseAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}

int svgIsVideoAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overlay"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transformBehavior"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    return type;
}


int svgIsCatchAllAttribute(const char* buff)
{
    int type = SVG_NULL;
    /* Catch All Properties */
    if     (!strcmp(buff, "audio-level"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "buffered-rendering"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "color"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "color-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "direction"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "display"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "display-align"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill-opacity"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill-rule"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-size"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "image-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "line-increment"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "opacity"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "pointer-events"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "shape-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "solid-color"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "solid-opacity"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stop-color"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stop-opacity"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-dasharray"))          { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-linecap"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-linejoin"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-miterlimit"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-opacity"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-width"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-align"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-anchor"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-rendering"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode-bidi"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "vector-effect"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewport-fill"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewport-fill-opacity"))     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "visibility"))                { type = SVG_CATCH_ALL; }
    /* Catch All Attributes */
    else if(!strcmp(buff, "about"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "accent-height"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "accumulate"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "additive"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "alphabetic"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "arabic-form"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ascent"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "attributeName"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "attributeType"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "bandwidth"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "baseProfile"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "bbox"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "by"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "calcMode"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cap-height"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "class"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "content"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "contentScriptType"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cx"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cy"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "d"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "defaultAction"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "descent"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "editable"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "end"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ev:event"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "event"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-stretch"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "from"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "g1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "g2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "glyph-name"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "gradientUnits"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "handler"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "hanging"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "height"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "horiz-adv-x"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "horiz-origin-x"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "id"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ideographic"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "k"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keyPoints"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keySplines"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keyTimes"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "lang"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mathematical"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "max"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaCharacterEncoding"))    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaContentEncodings"))     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaSize"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaTime"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "min"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "observer"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "offset"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "origin"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overlay"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overline-position"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overline-thickness"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "panose-1"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "path"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "pathLength"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "phase"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "playbackOrder"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "points"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "propagate"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "property"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "r"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "role"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rotate"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rx"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ry"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "slope"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "snapshotTime"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stemh"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stemv"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "strikethrough-position"))    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "strikethrough-thickness"))   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncBehaviorDefault"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncToleranceDefault"))      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "target"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "timelineBegin"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "to"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "transformBehavior"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "type"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "u1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "u2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "underline-position"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "underline-thickness"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode-range"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "units-per-em"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "values"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "version"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewBox"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "width"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "widths"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x-height"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "zoomAndPan"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "/"))                         { type = SVG_CATCH_ALL; }
    return type;
}

void svgProcess(int c, const char* buff)
{
    if(svgExpect == SVG_EXPECT_ELEMENT)
    {
        char advance = 0;
        if(buff[0] == '/') { return; }

        advance = svgIsElement(buff);

        if(advance) { printf("ELEMENT:\n"); svgExpect = SVG_EXPECT_ATTRIBUTE; currentElement = svgElement_create(buff); }
        else        { return; }
    }
    else if(svgExpect == SVG_EXPECT_ATTRIBUTE)
    {
        char advance = 0;
        const char* name;
        if(!currentElement) { /* TODO: error */ return; }
        name = currentElement->name;

        if(!strcmp(name, "?xml"))
        {
            if(!advance) { advance = svgIsXmlAttribute(buff); }
        }
        else if(!strcmp(name, "a"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsLinkAttribute(buff); }
        }
        else if(!strcmp(name, "animate"))
        {
            if(!advance) { advance = svgIsAnimateAttribute(buff); }
        }
        else if(!strcmp(name, "animateColor"))
        {
            if(!advance) { advance = svgIsAnimateColorAttribute(buff); }
        }
        else if(!strcmp(name, "animateMotion"))
        {
            if(!advance) { advance = svgIsAnimateMotionAttribute(buff); }
        }
        else if(!strcmp(name, "animateTransform"))
        {
            if(!advance) { advance = svgIsAnimateTransformAttribute(buff); }
        }
        else if(!strcmp(name, "animation"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsAnimationAttribute(buff); }
        }
        else if(!strcmp(name, "audio"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsAudioAttribute(buff); }
        }
        else if(!strcmp(name, "circle"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsCircleAttribute(buff); }
        }
        else if(!strcmp(name, "defs"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsDefsAttribute(buff); }
        }
        else if(!strcmp(name, "desc"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsDescAttribute(buff); }
        }
        else if(!strcmp(name, "discard"))
        {
            if(!advance) { advance = svgIsDiscardAttribute(buff); }
        }
        else if(!strcmp(name, "ellipse"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsEllipseAttribute(buff); }
        }
        else if(!strcmp(name, "font"))
        {
            if(!advance) { advance = svgIsFontAttribute(buff); }
        }
        else if(!strcmp(name, "font-face"))
        {
            if(!advance) { advance = svgIsFontFaceAttribute(buff); }
        }
        else if(!strcmp(name, "font-face-src"))
        {
            if(!advance) { advance = svgIsFontFaceSrcAttribute(buff); }
        }
        else if(!strcmp(name, "font-face-uri"))
        {
            if(!advance) { advance = svgIsFontFaceUriAttribute(buff); }
        }
        else if(!strcmp(name, "foreignObject"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsForeignObjectAttribute(buff); }
        }
        else if(!strcmp(name, "g"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsGroupAttribute(buff); }
        }
        else if(!strcmp(name, "glyph"))
        {
            if(!advance) { advance = svgIsGlyphAttribute(buff); }
        }
        else if(!strcmp(name, "handler"))
        {
            if(!advance) { advance = svgIsHandlerAttribute(buff); }
        }
        else if(!strcmp(name, "hkern"))
        {
            if(!advance) { advance = svgIsHKernAttribute(buff); }
        }
        else if(!strcmp(name, "image"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsImageAttribute(buff); }
        }
        else if(!strcmp(name, "line"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsLineAttribute(buff); }
        }
        else if(!strcmp(name, "linearGradient"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsLinearGradientAttribute(buff); }
        }
        else if(!strcmp(name, "listener"))
        {
            if(!advance) { advance = svgIsListenerAttribute(buff); }
        }
        else if(!strcmp(name, "metadata"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsMetadataAttribute(buff); }
        }
        else if(!strcmp(name, "missing-glyph"))
        {
            if(!advance) { advance = svgIsMissingGlyphAttribute(buff); }
        }
        else if(!strcmp(name, "mpath"))
        {
            if(!advance) { advance = svgIsMPathAttribute(buff); }
        }
        else if(!strcmp(name, "path"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsPathAttribute(buff); }
        }
        else if(!strcmp(name, "polygon"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsPolygonAttribute(buff); }
        }
        else if(!strcmp(name, "polyline"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsPolylineAttribute(buff); }
        }
        else if(!strcmp(name, "prefetch"))
        {
            if(!advance) { advance = svgIsPrefetchAttribute(buff); }
        }
        else if(!strcmp(name, "radialGradient"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsRadialGradientAttribute(buff); }
        }
        else if(!strcmp(name, "rect"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsRectAttribute(buff); }
        }
        else if(!strcmp(name, "script"))
        {
            if(!advance) { advance = svgIsScriptAttribute(buff); }
        }
        else if(!strcmp(name, "set"))
        {
            if(!advance) { advance = svgIsSetAttribute(buff); }
        }
        else if(!strcmp(name, "solidColor"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsSolidColorAttribute(buff); }
        }
        else if(!strcmp(name, "stop"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsStopAttribute(buff); }
        }
        else if(!strcmp(name, "svg"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsSvgAttribute(buff); }
        }
        else if(!strcmp(name, "switch"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsSwitchAttribute(buff); }
        }
        else if(!strcmp(name, "tbreak"))
        {
            if(!advance) { advance = svgIsTBreakAttribute(buff); }
        }
        else if(!strcmp(name, "text"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsTextAttribute(buff); }
        }
        else if(!strcmp(name, "textArea"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsTextAreaAttribute(buff); }
        }
        else if(!strcmp(name, "title"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsTitleAttribute(buff); }
        }
        else if(!strcmp(name, "tspan"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsTSpanAttribute(buff); }
        }
        else if(!strcmp(name, "use"))
        {
            if(!advance) { advance = svgIsProperty(buff); }
            if(!advance) { advance = svgIsUseAttribute(buff); }
        }
        else if(!strcmp(name, "video"))
        {
            if(!advance) { advance = svgIsMediaProperty(buff); }
            if(!advance) { advance = svgIsVideoAttribute(buff); }
        }

        if(advance) { printf("ATTRIBUTE:\n"); svgExpect = SVG_EXPECT_VALUE; free(currentAttribute); currentAttribute = emb_strdup(buff); }
    }
    else if(svgExpect == SVG_EXPECT_VALUE)
    {
        int last = strlen(buff) - 1;
        printf("VALUE:\n");

        /* single-value */
        if(buff[0] == '"' && (buff[last] == '/' || buff[last] == '"'))
        {
            svgExpect = SVG_EXPECT_ATTRIBUTE;
            svgElement_addAttribute(currentElement, svgAttribute_create(currentAttribute, buff));
        }
        else /* multi-value */
        {
            int last;
            if(!currentValue)
            {
                currentValue = emb_strdup(buff);
                if(!currentValue) { return; }
            }
            else
            {
                char* tmp = emb_strdup(currentValue);
                free(currentValue);
                currentValue = (char*)malloc(strlen(buff) + strlen(tmp) + 2);
                if(!currentValue) { return; }
                if(currentValue) memset(currentValue, 0, strlen(buff) + strlen(tmp) + 2);
                strcat(currentValue, tmp);
                strcat(currentValue, " ");
                strcat(currentValue, buff);
                free(tmp);
                tmp = 0;
            }

            last = strlen(buff) - 1;
            if(buff[last] == '/' || buff[last] == '"')
            {
                svgExpect = SVG_EXPECT_ATTRIBUTE;
                svgElement_addAttribute(currentElement, svgAttribute_create(currentAttribute, currentValue));
                free(currentValue);
                currentValue = 0;
            }
        }
    }

    if(svgExpect != SVG_EXPECT_NULL)
        printf("%s\n", buff);

    if(c == '>')
        svgExpect = SVG_EXPECT_NULL;
}

int readSvg(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int size = 1024;
    int pos;
    int c = 0;
    EmbRectObjectList* rList;
    EmbCircleObjectList* cList;
    EmbEllipseObjectList* eList;
    EmbLineObjectList* liList;
    EmbPointObjectList* poList;
    char* buff = (char*)malloc(size);
    if(!buff) { /* TODO: error */ return 0; }

    svgExpect = SVG_EXPECT_NULL;

    currentElement = 0;
    currentAttribute = 0;
    currentValue = 0;

    file = fopen(fileName, "r");
    if(file)
    {
        pos = 0;
        do
        {
            c = fgetc(file);
            switch(c)
            {
                case '<':
                    if(svgExpect == SVG_EXPECT_NULL)
                    {
                        svgAddToPattern(pattern);
                        svgExpect = SVG_EXPECT_ELEMENT;
                    }
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                case '=':
                case '>':
                    if(pos == 0)
                        break;
                    buff[pos] = 0;
                    pos = 0;
                    svgProcess(c, buff);
                    break;
                default:
                    buff[pos++] = (char)c;
                    break;
            }
            if(pos >= size - 1)
            {
                /* increase buff length - leave room for 0 */
                size *= 2;
                buff = (char*)realloc(buff,size);
                if(!buff) { /*TODO: error */ return 0; }
            }
        }
        while(c != EOF);
        fclose(file);
    }
    free(buff);
    free(currentAttribute);
    free(currentValue);

    /*TODO: remove this summary after testing is complete */
    printf("OBJECT SUMMARY:\n");
    cList = pattern->circleObjList;
    while(cList)
    {
        EmbCircle c = cList->circleObj.circle;
        printf("circle %f %f %f\n", c.centerX, c.centerY, c.radius);
        cList = cList->next;
    }
    eList = pattern->ellipseObjList;
    while(eList)
    {
        EmbEllipse e = eList->ellipseObj.ellipse;
        printf("ellipse %f %f %f %f\n", embEllipse_centerX(e), embEllipse_centerY(e), embEllipse_radiusX(e), embEllipse_radiusY(e));
        eList = eList->next;
    }
    liList = pattern->lineObjList;
    while(liList)
    {
        EmbLine li = liList->lineObj.line;
        printf("line %f %f %f %f\n", embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li));
        liList = liList->next;
    }
    poList = pattern->pointObjList;
    while(poList)
    {
        EmbPoint po = poList->pointObj.point;
        printf("point %f %f\n", embPoint_x(po), embPoint_y(po));
        poList = poList->next;
    }
    rList = pattern->rectObjList;
    while(rList)
    {
        EmbRect r = rList->rectObj.rect;
        printf("rect %f %f %f %f\n", embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r));
        rList = rList->next;
    }

    return 1; /*TODO: finish readSvg */
}

int writeSvg(EmbPattern* pattern, const char* fileName)
{
    EmbRect boundingRect;
    EmbStitchList* pointer;
    EmbCircleObjectList* cObjList = 0;
    EmbCircle circle;
    EmbEllipseObjectList* eObjList = 0;
    EmbEllipse ellipse;
    EmbLineObjectList* liObjList = 0;
    EmbLine line;
    EmbPointObjectList* poObjList = 0;
    EmbPoint point;
    EmbRectObjectList* rObjList = 0;
    EmbRect rect;

    FILE* file = fopen(fileName, "w");

    embPattern_flipVertical(pattern);
    boundingRect = embPattern_calcBoundingBox(pattern);
    fprintf(file, "<?xml version=\"1.0\"?>\n");
    fprintf(file, "<!-- Embroidermodder 2 SVG Embroidery File -->\n");
    fprintf(file, "<!-- http://embroidermodder.sourceforge.net -->\n");
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "version=\"1.2\" "
        "baseProfile=\"tiny\" "
    );

    /* TODO: See the SVG Tiny Version 1.2 Specification Section 7.14.
    *       Until all of the formats and API is stable, the width, height and viewBox attributes need to be left unspecified.
    *       If the attribute values are incorrect, some applications wont open it at all.
    fprintf(file, "viewBox=\"%f %f %f %f\" ",
            boundingRect.left,
            boundingRect.top,
            embRect_width(boundingRect),
            embRect_height(boundingRect)); */

    fprintf(file, ">");

    /*TODO: Low Priority Optimization:
    *      Using %g in fprintf just doesn't work good enough at trimming trailing zeroes.
    *      It's precision refers to significant digits, not decimal places (which is what we want).
    *      We need to roll our own function for trimming trailing zeroes to keep
    *      the precision as high as possible if needed, but help reduce file size also. */

    /*TODO: Low Priority Optimization:
    *      Make sure that the line length that is output doesn't exceed 1000 characters. */

    /*TODO: Low Priority: Indent output properly. */

    /* write circles */
    cObjList = pattern->circleObjList;
    while(cObjList)
    {
        circle = cObjList->circleObj.circle;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        fprintf(file, "\n<circle stroke-width=\"0.2\" stroke=\"#000000\" fill=\"none\" cx=\"%f\" cy=\"%f\" r=\"%f\" />",
                        circle.centerX,
                        circle.centerY,
                        circle.radius);
        cObjList = cObjList->next;
    }

    /* write ellipses */
    eObjList = pattern->ellipseObjList;
    while(eObjList)
    {
        ellipse = eObjList->ellipseObj.ellipse;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        fprintf(file, "\n<ellipse stroke-width=\"0.2\" stroke=\"#000000\" fill=\"none\" cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" />",
                        ellipse.centerX,
                        ellipse.centerY,
                        ellipse.radiusX,
                        ellipse.radiusY);
        eObjList = eObjList->next;
    }

    /* write lines */
    liObjList = pattern->lineObjList;
    while(liObjList)
    {
        line = liObjList->lineObj.line;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        fprintf(file, "\n<line stroke-width=\"0.2\" stroke=\"#000000\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                        line.x1,
                        line.y1,
                        line.x2,
                        line.y2);
        liObjList = liObjList->next;
    }

    /* write points */
    poObjList = pattern->pointObjList;
    while(poObjList)
    {
        point = poObjList->pointObj.point;
        /* See SVG Tiny 1.2 Spec:
        * Section 9.5 The 'line' element
        * Section C.6 'path' element implementation notes */
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        fprintf(file, "\n<line stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#000000\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                        point.xx,
                        point.yy,
                        point.xx,
                        point.yy);
        poObjList = poObjList->next;
    }

    /* write rects */
    rObjList = pattern->rectObjList;
    while(rObjList)
    {
        rect = rObjList->rectObj.rect;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        fprintf(file, "\n<rect stroke-width=\"0.2\" stroke=\"#000000\" fill=\"none\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />",
                        embRect_x(rect),
                        embRect_y(rect),
                        embRect_width(rect),
                        embRect_height(rect));
        rObjList = rObjList->next;
    }

    pointer = pattern->stitchList;
    if(embStitch_count(pointer) > 0)
    {
        while(pointer)
        {
            if((pointer->stitch.flags & TRIM) || pointer == pattern->stitchList)
            {
                EmbColor c;
                if(pointer != pattern->stitchList)
                {
                    fprintf(file, "\"/>");
                }

                c = embThread_getAt(pattern->threadList, pointer->stitch.color).color;
                /* TODO: use proper thread width for stoke-width rather than just 0.2 */
                fprintf(file, "\n<path stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" d=\"M %d %d",
                            c.r,
                            c.g,
                            c.b,
                            (int)((pointer->stitch.xx - boundingRect.left) * 10.0),
                            (int)((pointer->stitch.yy - boundingRect.top) * 10.0));
            }
            else
            {
                fprintf(file, " L %d %d",
                (int)((pointer->stitch.xx - boundingRect.left) * 10.0),
                (int)((pointer->stitch.yy - boundingRect.top) * 10.0));
            }
            pointer = pointer->next;
        }

        fprintf(file, "\"/>");
    }
    fprintf(file, "\n</svg>\n");
    fclose(file);

    /* Reset the pattern so future writes(regardless of format) are not flipped */
    embPattern_flipVertical(pattern);

    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
