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

#ifndef EMBROIDERMODDER_IMGUI_EXT_H
#define EMBROIDERMODDER_IMGUI_EXT_H

#include <string>

#include "imgui.h"

std::string translate(std::string string);

namespace ImGui {
    /* Need validation for the input number before applying to object.
    * and to add to undo history
    */
    inline void ShowDouble(std::string label, double data)
    {
        ImGui::Text("%s", translate(label).c_str());
        ImGui::SameLine();
        ImGui::Text("%s", std::to_string(data).c_str());
    }

    inline void TranslatedText(std::string str)
    {
        ImGui::Text("%s", translate(str).c_str());
    }
}

#endif
