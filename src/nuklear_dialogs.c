/*
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

void
message_box(char *title, char *message)
{
    GLFWwindow* window;

    if (!glfwInit()) {
        exit(3);
    }

    window = glfwCreateWindow(500, 200, "Message Box", NULL, NULL);
}
