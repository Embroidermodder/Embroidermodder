/*
 *  Embroidermodder 2.
 *  Testing for C core.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "core.h"

int
main(int argc, char *argv[])
{
    root = create_node(CNODE_TYPE_DICTIONARY);
    free_node(root);
    return 0;
}
