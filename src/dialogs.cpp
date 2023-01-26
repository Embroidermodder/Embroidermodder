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
    ImGui::Begin("About Embroidermodder 2");
    // ImageWidget img(assets_dir + "/images/logo-small");
    ImGui::Text("Embroidermodder " VERSION);
    ImGui::Text("https://www.libembroidery.org");
    ImGui::Text("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi.");
    ImGui::Text("Embroidery formats by Josh Varga and Robin Swift.");
    ImGui::Text("User Interface by Jonathan Greig and Robin Swift.");
    ImGui::Text("Free under the zlib/libpng license.");
    ImGui::End();
}

void
changelog_dialog(void)
{
    ImGui::Begin("Changelog");
    ImGui::End();
}

void
help_dialog(void)
{
    ImGui::Begin("Help");
    ImGui::End();    
}

void
alert(void)
{
    ImGui::Begin("Warning");
    ImGui::End();
}

void
details_dialog(void)
{
    ImGui::Begin("Design Details");
    ImGui::End();
}
