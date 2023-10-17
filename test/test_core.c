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

#include "core.h"

int
main(void)
{
    root = create_node(CNODE_TYPE_DICTIONARY);
	strcpy(root->key, "root");
	strcpy(root->data, "{}");
    CNode *leaf1 = create_and_add_leaf(root, "key1", "value1");
    CNode *leaf2 = create_and_add_leaf(root, "key2", "value2");
    CNode *leaf3 = create_and_add_leaf(root, "settings", "{}");
    CNode *leaf4 = create_and_add_leaf(leaf3, "key4", "value4");
    print_tree(root, 0);
	CNode *found = find_node(root, "key1");
	if (found != NULL) {
        print_tree(found, 0);
	}
	found = find_node(root, "settings.key4");
	if (found != NULL) {
        print_tree(found, 0);
	}
	found = find_node(root, "not-a-key");
	if (found != NULL) {
        print_tree(found, 0);
	}
    free_node(root);
    return 0;
}
