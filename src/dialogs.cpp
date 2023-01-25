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

#include "embroidermodder_imgui.h"

void
about_dialog(void)
{
    ImGui::BeginChild("About Embroidermodder 2");
    ImGui::EndChild();
}

void
changelog_dialog(void)
{
    ImGui::BeginChild("Changelog");
    ImGui::EndChild();
}

void
help_dialog(void)
{
    ImGui::BeginChild("Help");
    ImGui::EndChild();    
}

void
alert(void)
{
    ImGui::BeginChild("Warning");
    ImGui::EndChild();
}
