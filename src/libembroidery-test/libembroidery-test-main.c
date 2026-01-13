#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emb-reader-writer.h"
#include "emb-hash.h"

#define RED_TERM_COLOR "\e[0;31m"
#define GREEN_TERM_COLOR "\e[0;32m"
#define YELLOW_TERM_COLOR "\e[1;33m"
#define RESET_TERM_COLOR "\033[0m"

void todo(void)
{
    printf(YELLOW_TERM_COLOR "[TODO]\n" RESET_TERM_COLOR);
}

void pass(void)
{
    printf(GREEN_TERM_COLOR "[PASS]\n" RESET_TERM_COLOR);
}

int fail(int code)
{
    printf(RED_TERM_COLOR "[FAIL] [CODE=%d]\n" RESET_TERM_COLOR, code);
    return code;
}

void testRead(void)
{
    printf("Read Test...                      ");
    todo(); /* TODO: finish testRead() */
}

void testWrite(void)
{
    printf("Write Test...                     ");
    todo(); /* TODO: finish testWrite() */
}

void testHash(void)
{
    printf("Hash Test...                      ");
    EmbHash* hash = 0;
    hash = embHash_create();
    if(!hash) fail(1);
    if(!embHash_empty(hash)) fail(2);
    if(embHash_count(hash) != 0) fail(3);

    /* insert */
    if(embHash_insert(hash, "four", (void*)4)) fail(4);
    if(embHash_insert(hash, "five", (void*)5)) fail(5);
    if(embHash_insert(hash, "six",  (void*)6)) fail(6);
    if(embHash_count(hash) != 3) fail(7);

    /* replace */
    if(embHash_insert(hash, "four",  (void*)8)) fail(8);
    if(embHash_insert(hash, "five", (void*)10)) fail(10);
    if(embHash_insert(hash, "six",  (void*)12)) fail(12);
    if(embHash_count(hash) != 3) fail(13);

    /* contains */
    if(!embHash_contains(hash, "four")) fail(14);
    if(embHash_contains(hash, "empty")) fail(15);

    /* remove */
    embHash_remove(hash, "four");
    if(embHash_count(hash) != 2) fail(16);
    embHash_clear(hash);
    if(embHash_count(hash) != 0) fail(17);

    embHash_free(hash);
    pass();
}

int main(int argc, const char* argv[])
{
    /*TODO: Add tests here */

    testRead();
    testWrite();
    testHash();

    return 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
