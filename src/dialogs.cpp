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

#include "embroidermodder.h"

#include <cmath>
#include <string>
#include <fstream>

void
about_dialog(void)
{
    debug_message("about_dialog()");

    ImGui::Begin("About Embroidermodder 2");
    // ImageWidget img(assets_dir + "/images/logo-small");
    ImGui::Text("Embroidermodder " VERSION);
    ImGui::Text("https://www.libembroidery.org");
    ImGui::Text("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi.");
    ImGui::Text("Embroidery formats by Josh Varga and Robin Swift.");
    ImGui::Text("User Interface by Jonathan Greig and Robin Swift.");
    ImGui::Text("Free under the zlib/libpng license.");
    if (ImGui::Button("Oh, Yeah!")) {
        settings.show_about_dialog = false;
    }
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

    ImGui::Text("Help -- Embroidermodder 2");

    /* Minimum size (600, 500). */

    std::ifstream file;
    file.open(settings.assets_dir + "docs/manual.md");
    std::string s, line;
    while (std::getline(file, line)) {
        s += line + "\n";
    }

    ImGui::TextWrapped(s.c_str());

    /* Center button */
    if (ImGui::Button(translate("Close").c_str())) {

    }

    ImGui::End();    
}

void
alert(std::string title, std::string message)
{
    ImGui::Begin(title.c_str());
    ImGui::Text(message.c_str());
    if (ImGui::Button(translate("Ok").c_str())) {
        
    }
    ImGui::End();
}


/* TODO: Move majority of the calculation code into libembroidery
 *
 * Switch to grid layout.
 */
void
details_dialog(void)
{
    if (views.size() < 1) {
        return;
    }
    View view = views[settings.pattern_index];

    ImGui::Begin(translate("Embroidery Design Details").c_str());
    ImGui::SetWindowFontScale(1.5);

    EmbRect boundingRect = embPattern_calcBoundingBox(view.pattern);

    int colorChanges  = 0;

    if (view.pattern->stitchList->count == 0) {
        alert(translate("No Design Loaded"), translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }

    std::vector<double> stitchLengths;
    EmbArray *sts = view.pattern->stitchList;

    /*
    int colors = 0;
    double totalColorLength = 0.0;
    for (int i = 0; i < sts->count; i++) {
        EmbStitch st = sts->stitch[i];
        totalColorLength += length;
        if (i > 0 && sts->stitch[i-1].flags != NORMAL)
            length = 0.0; //can't count first normal stitch;
        if (sts->stitch[i].flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if (sts->stitch[i].flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    embPattern_lengthHistogram(sts, bin, NUMBINS);

    double binSize = max_stitchlength / NUMBINS;
    std::string str;
    for (int i = 0; i < NUMBINS; i++) {
        str += std::to_string(binSize * (i));
        str += " - " + std::to_string(binSize * (i+1));
        str += " mm: " +  std::to_string(bin[i]) + "\n\n";
    }

    ImGui::Text(translate("Stitch Distribution:").c_str());
    ImGui::Text(str.c_str());
    ImGui::Text(translate("Thread Length By Color:").c_str());
    int currentRow = 12;

    for (int i = 0; i < pattern->n_threads; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        debug_message("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(std::string::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }
    */

    /* setMinimumSize(750,550); */

    /* Collapsing header? */
    ImGui::Text(translate("General Information").c_str());

    ImGui::Text(translate("Total Stitches:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(view.pattern->stitchList->count).c_str());

    ImGui::Text(translate("Total Stitch Length:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(embPattern_totalStitchLength(view.pattern)).c_str());

    ImGui::Text(translate("Real Stitches:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(embPattern_realStitches(view.pattern)).c_str());

    ImGui::Text(translate("Jump Stitches:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(embPattern_jumpStitches(view.pattern)).c_str());

    ImGui::Text(translate("Trim Stitches:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(embPattern_trimStitches(view.pattern)).c_str());

    ImGui::Text(translate("Total Colors:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(view.pattern->n_threads).c_str());

    ImGui::Text(translate("Color Changes:").c_str());
    ImGui::SameLine();
    ImGui::Text(std::to_string(colorChanges).c_str());

    ImGui::Text(translate("Left:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.left) + " mm").c_str());

    ImGui::Text(translate("Top:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.top) + " mm").c_str());

    ImGui::Text(translate("Right:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.right)  + " mm").c_str());

    ImGui::Text(translate("Bottom:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.bottom) + " mm").c_str());

    ImGui::Text(translate("Width:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.right - boundingRect.left)  + " mm").c_str());

    ImGui::Text(translate("Height:").c_str());
    ImGui::SameLine();
    ImGui::Text((std::to_string(boundingRect.bottom - boundingRect.top) + " mm").c_str());

    if (ImGui::Button("Ok")) {
        show_details_dialog = false;
    }

    ImGui::End();

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram
}
