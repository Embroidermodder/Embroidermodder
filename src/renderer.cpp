/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "embroidermodder.h"

/* Description is a a simple string table of path instructions,
 * 
 */
int
render_vector_graphic(std::vector<std::string> description)
{
    return 0;
}

void
draw_rulers(void)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(
        ImVec2(0, 100),
        ImVec2(50, 640),
        IM_COL32(150, 170, 0, 255)
    );
    draw_list->AddRectFilled(
        ImVec2(0, 100),
        ImVec2(480, 150),
        IM_COL32(150, 170, 0, 255)
    );
    for (int i=0; i<20; i++) {
        draw_list->AddRectFilled(
            ImVec2(50+40*i, 100),
            ImVec2(50+40*i+1, 150),
            IM_COL32(0, 0, 0, 255)
        );
        for (int j=0; j<10; j++) {
            draw_list->AddRectFilled(
                ImVec2(50+40*i+4*j, 130),
                ImVec2(50+40*i+4*j+1, 150),
                IM_COL32(0, 0, 0, 255)
            );
        }
    }
    for (int i=0; i<10; i++) {
        draw_list->AddRectFilled(
            ImVec2(0, 150+40*i),
            ImVec2(50, 150+40*i+1),
            IM_COL32(0, 0, 0, 255)
        );
        for (int j=0; j<10; j++) {
            draw_list->AddRectFilled(
                ImVec2(30, 150+40*i+4*j),
                ImVec2(50, 150+40*i+4*j+1),
                IM_COL32(0, 0, 0, 255)
            );
        }
    }
}

void
draw_grid(void)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int i=0; i<10; i++) {
        draw_list->AddRectFilled(
            ImVec2(150+40*i, 150),
            ImVec2(150+40*i+1, 640),
            IM_COL32(0, 0, 0, 255)
        );
        draw_list->AddRectFilled(
            ImVec2(50, 150+40*i),
            ImVec2(640, 150+40*i+1),
            IM_COL32(0, 0, 0, 255)
        );
    }
}

void
pattern_view(void)
{
    EmbPattern *pattern = pattern_list[pattern_index];
    if (show_rulers) {
        draw_rulers();
    }
    if (show_grid) {
        draw_grid();
    }
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int i=1; i<pattern->stitchList->count; i++) {
        EmbStitch st1 = pattern->stitchList->stitch[i-1];
        EmbStitch st2 = pattern->stitchList->stitch[i];
        draw_list->AddLine(
            ImVec2(st1.x, st1.y),
            ImVec2(st2.x, st2.y),
            IM_COL32(0, 0, 0, 255)
        );
    }
}


#if 0
QPainterPath
add_to_path(QPainterPath path, char type, EmbVector offset, float *data, float scale)
{
    if (type == 'm') {
        path.moveTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'l') {
        path.lineTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'e') {
        path.addEllipse(EmbVector(offset.x+scale*data[0], offset.y-scale*data[1]),
            data[2]*scale, data[3]*scale);
    }
    if (type == 'a') {
        path.arcTo(offset.x+scale*data[0], offset.y-scale*data[1],
            data[2]*scale, data[3]*scale, data[4], data[5]);
    }
    return path;
}

QPainterPath
load_to_qpath(EmbVector offset, std::string str, float scale)
{
    QPainterPath path;

    char type = '0';
    int dot = 0;
    int index = 0;
    float position[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int first = 1;
    float sign = 1.0;
    for (size_t i=0; i<str.size(); i++) {
        char c = str[i];
        if ((c >= 'a') && (c <= 'z')) {
            dot = 0;
            index = 0;
            if (!first) {
                position[index] *= sign;
                path = add_to_path(path, type, offset, position, scale);
            }
            first = 0;
            type = c;
            sign = 1.0;
            for (int j=0; j<6; j++) {
                position[j] = 0.0;
            }
        }
        if ((c >= '0') && (c <= '9')) {
            int x = (int)(c - '0');
            if (dot) {
                position[index] = position[index] + pow(0.1, dot)*x;
                dot++;
            }
            else {
                position[index] = 10.0 * position[index] + 1.0*x;
            }
        }
        if (c == '-') {
            sign = -1.0;
        }
        if (c == '.') {
            dot = 1;
        }
        if (c == ',') {
            position[index] *= sign;
            index++;
            sign = 1.0;
            dot = 0;
        }
    }

    position[index] *= sign;
    path = add_to_path(path, type, offset, position, scale);

    return path;
}

void
createRulerTextPath(float x, float y, std::string str, float height)
{
    imdraw->AddText(ImVec2(x, y), 0xFFFFFFFF, str);
}
#endif
