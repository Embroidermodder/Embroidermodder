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

#include "embroidermodder_imgui.h"

std::string translate(std::string string)
{
    for (auto i : translations[language]) {
        if (i[0] == string) {
            return i[1];
        }
    }
    return string;
}

void debug_message(std::string string)
{
    if (debug_mode) {
        std::cout << string << std::endl;
    }
}
