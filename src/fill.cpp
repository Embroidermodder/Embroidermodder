/*!
 * \file generate.cpp
 * \brief Generate patterns without an existing design to fit to.
 */

#include "embroidermodder.h"

const char *fill_list[] = {
    [FILL_SATIN] = "satin",
    [FILL_GRADIENT] = "gradient",
    [FILL_BRICK] = "brick",
    [N_FILLS] = "_END"
};

void
MainWindow::fill(const char *type)
{
    int id = get_id(fill_list, type);
    switch (id) {
    case -1:
    default:
        qDebug("ERROR: failed to identify fill pattern.");
        break;
    }
}

