/*!
 * \file generate.cpp
 * \brief Generate patterns without an existing design to fit to.
 */

#include "embroidermodder.h"

void
MainWindow::fill(const char *type)
{
    int id = get_id(state.fill_list, type);
    switch (id) {
    case -1:
    default:
        debug("ERROR: failed to identify fill pattern.");
        break;
    }
}

