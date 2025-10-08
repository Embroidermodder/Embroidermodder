/*!
 * \file generate.cpp
 * \brief Generate patterns without an existing design to fit to.
 */

#include "embroidermodder.h"

const char *generate_list[] = {
    [GEN_PHOTO] = "photo",
    [GEN_DRAWING] = "drawing",
    [GEN_QR] = "qr",
    [GEN_GUILLOCHE] = "guilloche",
    [GEN_KNOT] = "knot",
    [N_GEN] = "_END"
};

void
MainWindow::generate(const char *type)
{
    int id = get_id(generate_list, type);
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
        qDebug("ERROR: failed to identify generation pattern.");
        break;
    }
}

