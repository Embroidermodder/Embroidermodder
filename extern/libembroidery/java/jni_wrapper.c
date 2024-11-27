/*
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ---------------------------------------------------------
 *
 * The bindings for libembroidery on Java: for Android app support.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

#include "embroidery.h"

EmbPattern *patterns[100];

int c_emb_identify_format(const char *ending)
{

}

int c_embThread_findNearestColor(EmbColor color, EmbArray* colors, int mode)
{

}

int c_embThread_findNearestColor_fromThread(EmbColor color, EmbThread* colors, int length)
{

}

EmbThread c_embThread_getRandom(void)
{

}

void c_testMain(int level)
{

}

int c_convert(const char *inf, const char *outf)
{

}

EmbColor c_embColor_make(unsigned char r, unsigned char g, unsigned char b)
{

}

EmbColor* c_embColor_create(unsigned char r, unsigned char g, unsigned char b)
{

}

EmbColor c_embColor_fromHexStr(char* val)
{

}

int c_embColor_distance(EmbColor a, EmbColor b)
{

}

EmbEllipse c_embEllipse_make(double cx, double cy, double rx, double ry)
{

}

int c_threadColor(const char*, int brand)
{

}

int c_threadColorNum(unsigned int color, int brand)
{

}

const char* c_threadColorName(unsigned int color, int brand)
{

}

void c_embSatinOutline_generateSatinOutline(EmbArray* lines, double thickness, EmbSatinOutline* result)
{

}

EmbArray* c_embSatinOutline_renderStitches(EmbSatinOutline* result, double density)
{

}

int c_embPattern_create(void)
{

}

void c_embPattern_hideStitchesOverLength(int p, int length)
{

}

void c_embPattern_fixColorCount(int p)
{

}

int c_embPattern_addThread(int p, EmbThread thread)
{

}

void c_embPattern_addStitchAbs(int p, double x, double y, int flags, int isAutoColorIndex)
{

}

void c_embPattern_addStitchRel(int p, double dx, double dy, int flags, int isAutoColorIndex)
{

}

void c_embPattern_changeColor(int p, int index)
{

}

void c_embPattern_free(int p)
{

}

void c_embPattern_scale(int p, double scale)
{

}

EmbRect embPattern_calcBoundingBox(int p)
{

}

void c_embPattern_flipHorizontal(int p);

void c_embPattern_flipVertical(int p);

void c_embPattern_flip(int p, int horz, int vert);

void c_embPattern_combineJumpStitches(int p);

void c_embPattern_correctForMaxStitchLength(int p, double maxStitchLength, 
double maxJumpLength);

void c_embPattern_center(int p)
{

}

void c_embPattern_loadExternalColorFile(int p, const char* fileName)
{
    
}

void c_embPattern_convertGeometry(int p);

void c_embPattern_designDetails(int p);

int embPattern_combine(int p1, int p2);

int c_embPattern_color_count(int pattern, EmbColor startColor);

void c_embPattern_end(int p);

void c_embPattern_crossstitch(int pattern, int , int threshhold);

void c_embPattern_horizontal_fill(int pattern, int , int threshhold);

int c_embPattern_render(int pattern, char *fname);

int c_embPattern_simulate(int pattern, char *fname)
{

}

void c_embPattern_addCircleAbs(int p, EmbCircle obj)
{

}

void c_embPattern_addEllipseAbs(int p, EmbEllipse obj)
{

}

void c_embPattern_addLineAbs(int p, EmbLine obj)
{

}

void c_embPattern_addPathAbs(int p, EmbPath obj)
{

}

void c_embPattern_addPointAbs(int p, EmbPoint obj)
{

}

void c_embPattern_addPolygonAbs(int p, EmbPolygon obj)
{

}

void c_embPattern_addPolylineAbs(int p, EmbPolyline obj)
{

}

void c_embPattern_addRectAbs(int p, EmbRect obj)
{

}

void c_embPattern_copyStitchListToPolylines(int pattern)
{

}

void c_embPattern_copyPolylinesToStitchList(int pattern)
{

}

void c_embPattern_moveStitchListToPolylines(int pattern)
{

}

void c_embPattern_movePolylinesToStitchList(int pattern)
{

}

char c_embPattern_read(int pattern, const char* fileName, int format)
{

}

char c_embPattern_write(int pattern, const char* fileName, int format)
{

}

char c_embPattern_readAuto(int pattern, const char* fileName)
{

}

char c_embPattern_writeAuto(int pattern, const char* fileName)
{

}

void c_report(int result, char *label)
{

}

int c_full_test_matrix(char *fname)
{

}

EmbImage c_embImage_create(int, int)
{

}

void c_embImage_read(int image, char *fname)
{

}

int c_embImage_write(int image, char *fname)
{

}

void c_embImage_free(int image)
{

}
