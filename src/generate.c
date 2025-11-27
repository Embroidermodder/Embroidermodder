/*
 * Generate patterns without an existing design to fit to.
 */

#include "core.h"

EmbArray *
generate(const char *type)
{
    int id = get_index(state.generate_list, type);
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
    return NULL;
}

