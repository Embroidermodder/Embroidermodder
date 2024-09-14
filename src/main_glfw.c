/*
 * Embroidermodder 2 (GLFW version)
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 */

#include <stdio.h>

#include "../extern/glfw/include/GLFW/glfw3.h"

#include "core.h"

/* . */
void
init_command(void)
{

}

/* . */
void
clear_selection(void)
{

}

/* . */
void
end_command(void)
{

}

/* . */
void
prompt_output(EmbString line)
{
    printf("%s\n", line);
}

/* . */
void
draw_quad(float desc[7])
{
    glColor3f(desc[0], desc[1], desc[2]);
    glBegin(GL_QUADS);
    glVertex2f(desc[3], desc[5]);
    glVertex2f(desc[3], desc[6]);
    glVertex2f(desc[4], desc[6]);
    glVertex2f(desc[4], desc[5]);
    glEnd();
}

/* . */
void
draw_interface(void)
{
    float desc[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0};
    draw_quad(desc);
}

/* . */
int
make_application(int argc, char *argv[])
{
    GLFWwindow* window;

    if (!load_data()) {
        return 1;
    }

    if (!glfwInit()) {
        return 2;
    }

    window = glfwCreateWindow(640, 480, "Embroidermodder 2 (GLFW)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 3;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, 640, 480);

        draw_interface();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

