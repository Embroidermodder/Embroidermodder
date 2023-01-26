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

#include "embroidermodder_imgui.h"

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
