/*!
 * \file generate.cpp
 * \brief Generate patterns without an existing design to fit to.
 */

#include "embroidermodder.h"

void
MainWindow::generate(const char *type)
{
    int id = get_id(state.generate_list, type);
    switch (id) {
    case GEN_PHOTO: {
        break;
    }
    case GEN_DRAWING: {
        break;
    }
    case GEN_QR: {
        break;
    }
    case GEN_GUILLOCHE: {
        break;
    }
    case GEN_KNOT: {
        break;
    }
    case -1:
    default:
        debug("ERROR: failed to identify generation pattern.");
        break;
    }
}

