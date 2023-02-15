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

namespace ImGui {
    void ShowDouble(std::string label, double data);
    void TranslatedText(std::string str);
    void CheckboxData(std::string str, bool *stor, bool *value);
    void EditDouble(std::string str, double *data);
}

#endif
