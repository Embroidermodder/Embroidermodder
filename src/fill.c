/*
 * Generate patterns without an existing design to fit to.
 */

#include "core.h"

EmbArray *
fill(const char *type)
{
    int id = get_index(state.fill_list, type);
    switch (id) {
    case -1:
    default:
        debug("ERROR: failed to identify fill pattern.");
        break;
    }
    return NULL;
}

