/*
 * Embroidermodder 2 -- Tree
 * Copyright 2011-2026 The Embroidermodder Team
 */

/*
 * TODO: tests for this file.
 */

#include <stdlib.h>
#include <string.h>

#include "core.h"

Node *root_node(void)
{
    Node *root = malloc(sizeof(Node));
    root->leaves = malloc(sizeof(Node));
    root->n_leaves = 0;
    root->type = NODE_ROOT;
    sprintf(root->label, "root");
    return root;
}

Node *create_leaf(Node *trunk)
{
    trunk->n_leaves++;
    trunk->leaves = realloc(trunk->leaves, sizeof(Node)*(trunk->n_leaves));

    Node *node = trunk->leaves + (trunk->n_leaves-1);
    node->type = NODE_UNKNOWN;
    node->leaves = malloc(sizeof(Node));
    node->n_leaves = 0;
    return node;
}

Node *add_dict_leaf(Node *trunk, const char *label)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_DICTIONARY;
    sprintf(leaf->label, "%s", label);
    return leaf;
}

void add_str_leaf(Node *trunk, const char *label, const char *str)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_STR;
    sprintf(leaf->label, "%s", label);
    leaf->s = str_create(str);
}

/* NOTE: When creating a StrArray leaf node it begins empty. */
void add_strarray_leaf(Node *trunk, const char *label)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_STR;
    sprintf(leaf->label, "%s", label);
    leaf->l = strarray_create();
}

void add_int_leaf(Node *trunk, const char *label, int32_t i)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_INT;
    sprintf(leaf->label, "%s", label);
    leaf->i = i;
}

void add_uint_leaf(Node *trunk, const char *label, uint32_t u)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_UINT;
    sprintf(leaf->label, "%s", label);
    leaf->u = u;
}

void add_real_leaf(Node *trunk, const char *label, double r)
{
    Node *leaf = create_leaf(trunk);
    leaf->type = NODE_REAL;
    sprintf(leaf->label, "%s", label);
    leaf->r = r;
}

/* Find an exact match to the key as the label of a node on this level. */
Node *find_leaf(Node *trunk, const char *key)
{
    return NULL;
}

/* Find an exact match to the key as the label of a node on any lower level. */
Node *recursive_find_leaf(Node *root, const char *key)
{
    return NULL;
}

/* Find leaf given the full path in "dot.notation" from the root. */
Node *get_leaf(Node *root, const char *key)
{
    int i;
    char subkey[100];
    const char *tail;
    int last_level = 1;
    for (i=0; key[i]; i++) {
        subkey[i] = key[i];
        if (key[i] == '.') {
            subkey[i] = 0;
            last_level = 0;
            tail = key+i+1;
            break;
        }
    }
    subkey[i] = 0;

    for (i=0; i<root->n_leaves; i++) {
        if (!strcmp(root->leaves[i].label, subkey)) {
            if (last_level) {
                return root->leaves + i;
            }
            else {
                return get_leaf(root->leaves + i, tail);
            }
        }
    }

    return NULL;
}

Node *recursive_copy_node(Node *node)
{
    return NULL;
}

Node *pop_leaf(Node *trunk)
{
    Node *result = recursive_copy_node(trunk->leaves + (trunk->n_leaves-1));
    free_leaf(trunk->leaves + (trunk->n_leaves-1));
    trunk->n_leaves--;
    return result;
}

void free_leaf(Node *trunk)
{
    while (trunk->n_leaves > 0) {
        free_leaf(trunk->leaves + (trunk->n_leaves-1));
        trunk->n_leaves--;
    }
    if (trunk->type == NODE_STR) {
        str_free(trunk->s);
    }
    if (trunk->type == NODE_STRARRAY) {
        strarray_free(trunk->l);
    }
    free(trunk->leaves);
}

#define BETWEEN_TAGS                  0
#define IN_TAG                        1
#define IN_DATA                       2

/* HACK: this isn't a proper xml parser because we just require
 * that no tags appear in the data text. No escaping etc.
 *
 * NOTE: this is a destructive parser.
 *
 * TODO: restructure so it's less nested using states.
 */
void
parse_xml(Node *result, uint8_t *start, uint8_t *end)
{
    int state = BETWEEN_TAGS;
    uint8_t *ptr;
    uint8_t *current_tag;
    uint8_t *current_data;
    for (ptr=start; ptr<end; ptr++) {
        if (*ptr == '<') {
            /* Zero-terminate current tag. */
            current_tag = ptr+1;
            for (; ptr!=end; ptr++) {
                if (*ptr == '>') {
                    *ptr = 0;
                    current_data = ptr+1;
                    break;
                }
            }

            /* Zero-terminate current data. */
            int contains_tags = 0;
            uint8_t *data_end = end;
            for (; ptr<end; ptr++) {
                printf("%c", *ptr);
                if (*ptr == '<') {
                    if (*(ptr+1) == '/') {
                        if (!strncmp(current_tag, ptr+2, strlen(current_tag))) {
                            *ptr = 0;
                            data_end = ptr;
                            break;
                        }
                    }
                    else {
                        contains_tags = 1;
                    }
                }
            }

            /* Add to tree in memory. */
            if (contains_tags) {
                Node *leaf = add_dict_leaf(result, current_tag);
                parse_xml(leaf, current_data, data_end);
            }
            else {
                add_str_leaf(result, current_tag, current_data);
            }
        }
    }
}

uint8_t *load_file(const char *fname, size_t *length)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        return NULL;
    }
    uint8_t *data = calloc(1000000, sizeof(uint8_t));
    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(data, 1, *length, f);
    fclose(f);
    return data;
}

Node *load_xml(const char *fname)
{
    size_t length = 0;
    uint8_t *data = load_file(fname, &length);
    if (data == NULL) {
        printf("ERROR: failed to load file \"%s\"\n", fname);
        return NULL;
    }

    Node *result = root_node();
    parse_xml(result, data, data+length);
    free(data);
    return result;
}

/* Find a <position> tag in this element and report it.
 * If none are found, return -1.
 */
int get_position(Node *item)
{
    Node *leaf = get_leaf(item, "position");
    if (leaf == NULL) {
        return -1;
    }
    return atoi(leaf->s->data);
}

/* Find a <data> tag in this element and report it.
 * If none are found, return NULL.
 */
String *get_data(Node *item)
{
    Node *leaf = get_leaf(item, "data");
    if (leaf == NULL) {
        return NULL;
    }
    return leaf->s;
}

/* Custom loader for StrArrays.
 *
 * NOTE: Memory is not assigned here.
 */
int strarray_from_tree(Node *tree, const char *key, StrArray *array)
{
    int i;
    Node *data = get_leaf(tree, key);
    if (data == NULL) {
        printf("ERROR: failed to find key \"%s\" in tree.\n", key);
        return 0;
    }
    for (int i=0; i<data->n_leaves; i++) {
        int position = get_position(data->leaves + i);
        String *content = get_data(data->leaves + i);

        strarray_insert(array, position, content);
    }
    return 1;
}

/* Render the tree to the console so we can check for errors. */
void print_tree(Node *root, int indent)
{
    int i;
    for (i=0; i<indent; i++) {
        printf("|");
    }
    switch (root->type) {
    default:
    case NODE_UNKNOWN:
        printf("%s: UNKNOWN TYPE\n", root->label);
        break;
    case NODE_ROOT:
    case NODE_DICTIONARY:
        printf("%s:\n", root->label);
        for (i=0; i<root->n_leaves; i++) {
            print_tree(root->leaves + i, indent+1);
        }
        break;
    case NODE_STR:
        printf("%s: %s\n", root->label, root->s->data);
        break;
    case NODE_INT:
        printf("%s: %d\n", root->label, root->i);
        break;
    case NODE_UINT:
        printf("%s: %d\n", root->label, root->u);
        break;
    case NODE_REAL:
        printf("%s: %f\n", root->label, root->r);
        break;
    }
}

/* Run a battery of tests that constructs trees then tests to see if
 * their contents match.
 *
 * This can be called from the command line with:
 *     $ ./embroidermodder2 --core-test
 */
int tree_test(void)
{
    Node *example = load_xml("config.xml");
    print_tree(example, 0);
    return 1;
}
