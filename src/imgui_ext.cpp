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

#include "embroidermodder.h"

#include "imgui.h"

namespace ImGui {
    void Text(std::string str)
    {
        ImGui::Text("%s", str.c_str());
    }

    /* Need validation for the input number before applying to object.
    * and to add to undo history
    */
    void ShowDouble(std::string label, double data)
    {
        ImGui::Text("%s", translate(label).c_str());
        ImGui::SameLine();
        ImGui::Text("%s", std::to_string(data).c_str());
    }

    void TranslatedText(std::string str)
    {
        ImGui::Text("%s", translate(str).c_str());
    }

    void CheckboxData(std::string label, bool *stor, bool *value)
    {
        *stor = *value;
        ImGui::Checkbox(translate(label).c_str(), stor);
        *value = *stor;
    }

    void EditDouble(std::string label, double *data)
    {
        ImGui::InputDouble(translate(label).c_str(), data, *data);
    }
}
