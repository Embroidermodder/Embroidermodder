/*
 * This file is part of the Embroidermodder PET.
 *
 * Copyright 2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ---------------------------------------------------------
 */

#ifndef PET_INTERFACE_HEADER
#define PET_INTERFACE_HEADER

#include "embroidery.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sets the state globally. */
void set_pen();
void set_text_style();

/* Drawing functions. */
void line(int x1, int y1, int x2, int y2);
void rectangle(int x, int y, int w, int h);
void polygon(int *a, int length);
void text(char *msg, int x, int y, int wordwrap);

/* User interface. */
int menu(void);
int user_interface(int button_state[]);

#ifdef __cplusplus
}
#endif

#endif
