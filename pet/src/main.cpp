/*
 * This file is part of the Embroidermodder PET.
 *
 * Copyright 2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ---------------------------------------------------------
 */

#include "hardware/gpio.h"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "tufty2040.hpp"

#include "interface.h"

int button_state[10];

using namespace pimoroni;

Tufty2040 tufty;
ST7789 st7789(
    Tufty2040::WIDTH,
    Tufty2040::HEIGHT,
    ROTATE_0,
    ParallelPins{
        Tufty2040::LCD_CS,
        Tufty2040::LCD_DC,
        Tufty2040::LCD_WR,
        Tufty2040::LCD_RD,
        Tufty2040::LCD_D0,
        Tufty2040::BACKLIGHT
    }
);
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

//Button button_a(Tufty2040::A, Polarity::ACTIVE_HIGH);

/* Wrappers for PicoGraphics class functions for C. */
void
text(char *msg, int x, int y, int wordwrap)
{
    graphics.text(msg, Point(x, y), wordwrap);
}

void
line(int x1, int y1, int x2, int y2)
{
    graphics.line(Point(x1, y1), Point(x2, y2));
}

void
rectangle(int x, int y, int w, int h)
{
    graphics.rectangle(Rect(x, y, w, h));
}

void
polygon(int *a, int length)
{
}

void
pixel(int x, int y)
{
    graphics.pixel(Point(x, y));
}

int
main(void)
{
    Pen bg = graphics.create_pen(255, 0, 0);
    Pen fg = graphics.create_pen(0, 0, 0);
    st7789.set_backlight(255);
    graphics.set_font("bitmap8");
    gpio_init(Tufty2040::A);

    while (true) {
        graphics.set_pen(bg);
        graphics.clear();
        graphics.set_pen(fg);
//        user_interface(button_state);
//        pixel(0, 0);
//        line(10, 20, 20, 20);
//        text("hello world", 30, 40, 100);

        if (gpio_get(Tufty2040::A) > 0) {
           rectangle(10, 20, 20, 20);
        }
        graphics.pixel(Point(10, 10));
        graphics.pixel(Point(20, 30));
        st7789.update(&graphics);
    }
}
