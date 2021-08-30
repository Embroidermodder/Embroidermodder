/* Embroidermodder 2.
 * ------------------------------------------------------------
 * Copyright 2021 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE.txt for licensing terms.
 * ------------------------------------------------------------
 * This file is for the functions, not the data, of embroidermodder 2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__unix__) || defined(__linux__)
#include <pwd.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "GL/freeglut.h"
#include "cJSON.h"

#include "embroidermodder.h"

/* DATA SECTION */
texture_t tex[N_TEXTURES];
GLuint texture[N_TEXTURES];
char *folders[] = {
    "",
    "commands",
    "help",
    "icons",
    "images",
    "samples",
    "translations"
};
int interaction_mode = 0;
int run = 1;
int window_width = 640;
int window_height = 480;
float mouse[2];
int mouse_x = 0;
int mouse_y = 0;

void key_handler(int c, int x, int y);
void render_quadlist(quad *qlist);
void menu(int key);
void display(void);

action_hash_data menu_actions[] = {
    { ACTION_delete, ICON_erase, "D&elete", "Delete", "Removes objects from a drawing." }
};

/* FUNCTIONS SECTION */

int main_tex_example(int argc, char *argv[])
{
    int window;
    int rightclick_menu;
    int file_menu, edit_menu, settings_menu, window_menu, help_menu;
    int i, ntextures;
    puts("SDL2 version of Embroidermodder");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100,100);
    window = glutCreateWindow("Embroidermodder 2 (SDL)");
    glClearColor (0.5, 0.5, 0.5, 0.0);
    
    file_menu = glutCreateMenu(menu);
    glutAddMenuEntry("New", ACTION_new);
    glutAddMenuEntry("Open", ACTION_open);
    glutAddMenuEntry("Save", ACTION_save);
    glutAddMenuEntry("Save as", ACTION_saveas);
    glutAddMenuEntry("Exit", ACTION_exit);
    edit_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Undo", ACTION_undo);
    settings_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Undo", ACTION_undo);
    window_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Undo", ACTION_undo);
    help_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Undo", ACTION_undo);
    rightclick_menu = glutCreateMenu(menu);
    glutAddSubMenu("File", file_menu);
    glutAddSubMenu("Edit", edit_menu);
    glutAddSubMenu("Settings", settings_menu);
    glutAddSubMenu("Window", window_menu);
    glutAddSubMenu("Help", help_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    ntextures = 2;
    glGenTextures(N_TEXTURES, texture);
    for (i=0; i<N_TEXTURES; i++) {
        generate_texture(i);
    }
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutKeyboardFunc(key_handler);
    glutMainLoop();
    return 0;
}

void app_dir(char *output, int folder)
{
#if defined(__unix__) || defined(__linux__)
    char *separator = "/";

    strcpy(output, getenv("HOME"));

    /* On MacOS we set a system "HOME" manually if it is not set. */
    if (!output) {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd) {
            output = pwd->pw_dir;
        }
        else {
            printf("ERROR: failed to set HOME.");
        }
    }

#else
    char *separator = "\\";

    strcpy(output, getenv("HOMEDRIVE"));
    strcat(output, getenv("HOMEPATH"));
#endif

    strcat(output, separator);
    strcat(output, ".embroidermodder2");
    strcat(output, separator);

    if (folder >= 0 && folder < nFolders) {
        strcat(output, folders[folder]);
        strcat(output, separator);
    }
}

/* UTILITY FUNCTIONS FOR ALL SYSTEMS */
void debug_message(const char *format, ...)
{
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}

int file_exists(char *fname)
{
    struct stat stats;
    return !stat(fname, &stats);
}

int loadJSON(char *fname)
{
    if (file_exists(fname)) {
        /* Config has been written, load to settings. */
        parseJSON(fname);
    }
    else {
        /* We load from the defaults instead. */
    }
    return 0;
}

int saveJSON(char *fname)
{
    /* Write all current values of settings to JSON file. */
    return 0;
}

int parseJSON(char *fname)
{
    FILE *f;
    char str[1000];
    cJSON *obj, *name, *value;
    int length;

    puts("Embroidermodder sandbox.");

    f = fopen(fname, "rb");
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(str, 1, length, f);
    fclose(f);

    obj = cJSON_Parse(str);
    if (!obj) {
        puts("Failed to parse JSON string.");
        const char *error = cJSON_GetErrorPtr();
        printf("%s\n", error);
        return 1;
    }
    name = cJSON_GetObjectItemCaseSensitive(obj, "File");
    if (!name) {
        printf("Failed find key File.");
        return 0;
    }
    printf("name: %d\n", name->type);
    value = cJSON_GetObjectItemCaseSensitive(name, "type");
    if (!value) {
        printf("Failed find key type within dict File.");
        return 0;
    }
    printf("name: %d\n", value->type);

    if (cJSON_IsString(value) && value->valuestring) {
        printf("%s\n", value->valuestring);
    }

    cJSON_Delete(obj);
    return 0;
}

void render_quadlist(quad *qlist)
{
    int i;
    glViewport(0, 0, window_width, window_height);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (i=0; qlist[i].flag; i++) {
        glColor3f(qlist[i].red, qlist[i].green, qlist[i].blue);
        glBegin(GL_QUADS);
        glVertex2f(qlist[i].left, qlist[i].top);
        glVertex2f(qlist[i].left, qlist[i].bottom);
        glVertex2f(qlist[i].right, qlist[i].bottom);
        glVertex2f(qlist[i].right, qlist[i].top);
        glEnd();
    }
}

void menu(int key)
{
    switch (key) {
    default:
        break;
    }
}

void display()
{
    /* render_quadlist(quad_list1); */
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (i=0; i<N_TEXTURES; i++) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBegin(GL_QUADS);
        glTexCoord2f(tex[i].position[0], tex[i].position[1]);
        glVertex2f(tex[i].corners[0], tex[i].corners[1]);
        glTexCoord2f(tex[i].position[2], tex[i].position[3]);
        glVertex2f(tex[i].corners[2], tex[i].corners[3]);
        glTexCoord2f(tex[i].position[4], tex[i].position[5]);
        glVertex2f(tex[i].corners[4], tex[i].corners[5]);
        glTexCoord2f(tex[i].position[6], tex[i].position[7]);
        glVertex2f(tex[i].corners[6], tex[i].corners[7]);
        glEnd();
    }
    glutSwapBuffers();
}

void key_handler(int key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
    default:
        break;
    }
}

void generate_texture(int i)
{
    unsigned char data[128*128*3];
    int j;
    for (j=0; j<128*128; j++) {
        data[3*j] = j%256;
        data[3*j+1] = j%256;
        data[3*j+2] = j%256;
    }
    tex[i].width = 128;
    tex[i].height = 128;
    tex[i].corners[0] = 0.0;
    tex[i].corners[1] = 0.0;
    tex[i].corners[2] = 0.0;
    tex[i].corners[3] = 1.0;
    tex[i].corners[4] = 1.0;
    tex[i].corners[5] = 1.0;
    tex[i].corners[6] = 1.0;
    tex[i].corners[7] = 0.0;
    tex[i].position[0] = 0.0;
    tex[i].position[1] = 0.0;
    tex[i].position[2] = 0.0;
    tex[i].position[3] = 1.0;
    tex[i].position[4] = 1.0;
    tex[i].position[5] = 1.0;
    tex[i].position[6] = 1.0;
    tex[i].position[7] = 0.0;
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex[i].width, tex[i].height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data);
}


