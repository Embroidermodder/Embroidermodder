/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Wrapper for system-specific UI features.
 * This is an alternative approach to using GLFW.
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#if __WIN32__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#endif

#include "core.h"

void
draw_frame(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glEnd();
}

#ifdef __WIN32__
/* Warning: this is untested boilerplate. */

int APIENTRY
wWinMain(_In_     HINSTANCE hInstance,
         _In_opt_ HINSTANCE hPrevInstance,
         _In_     LPWSTR    lpCmdLine,
         _In_     int       nCmdShow)
{
    char title[200];
    sprintf(title, "%s %s", _appName_, _appVer_);
    createWindowW(title, title, 0, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DisplatchMessage(&msg);
    }

    return 0;
}

#else
/* All systems other than __WIN32__ are assumed to be X11.
 * MacOS still supports X11: GIMP and Inkscape still use it.
 * In future we may have Wayland support.
 */

int
main(int argc, char *argv[])
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        printf("ERROR: failed to open an X11-compatible display.");
        return 0;
    }

    Window root = DefaultRootWindow(display);

    glEnable(GL_DEPTH_TEST);

    XSetWindowAttributes attributes;
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *visual = glXChooseVisual(display, 0, att);

    attributes.colormap = XCreateColormap(display, root, visual->visual, AllocNone);
    attributes.event_mask = ExposureMask | ButtonPressMask | PointerMotionMask;

    Window win = XCreateWindow(display, root, 100, 100, 640, 480, 0,
        visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask,
        &attributes);
    XMapWindow(display, win);
    char title[200];
    sprintf(title, "%s %s", _appName_, _appVer_);
    XStoreName(display, win, title);

    GLXContext context = glXCreateContext(display, visual, NULL, GL_TRUE);
    glXMakeCurrent(display, win, context);

    int running = 1;
    while (running) {
        XWindowAttributes attr;
        XEvent event;
        XNextEvent(display, &event);
        XGetWindowAttributes(display, win, &attr);
        switch (event.type) {
        default:
            break;
        }
        draw_frame();
        glXSwapBuffers(display, win);
    }

    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}

#endif

