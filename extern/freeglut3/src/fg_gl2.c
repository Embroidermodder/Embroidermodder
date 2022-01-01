/*
 * fg_gl2.c
 *
 * Load OpenGL (ES) 2.0 functions used by fg_geometry
 *
 * Copyright (C) 2012  Sylvain Beucler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <GL/freeglut.h>
#include "fg_internal.h"
#include "fg_gl2.h"

void FGAPIENTRY glutSetVertexAttribCoord3(GLint attrib) {
  if (fgStructure.CurrentWindow != NULL)
    fgStructure.CurrentWindow->Window.attribute_v_coord = attrib;
}

void FGAPIENTRY glutSetVertexAttribNormal(GLint attrib) {
  if (fgStructure.CurrentWindow != NULL)
    fgStructure.CurrentWindow->Window.attribute_v_normal = attrib;
}

void FGAPIENTRY glutSetVertexAttribTexCoord2(GLint attrib) {
    if (fgStructure.CurrentWindow != NULL)
        fgStructure.CurrentWindow->Window.attribute_v_texture = attrib;
}

void fgInitGL2() {
    fgState.HasOpenGL20 = (fgState.MajorVersion >= 2);
}

