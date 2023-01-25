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

#include <iostream>

void
actuator(std::string command)
{
    if (command == "new") {
        std::cout << "New File" << std::endl;
    }
    if (command == "open") {
        std::cout << "Open File" << std::endl;
    }
    if (command == "quit") {
        running = false;
    }
}
