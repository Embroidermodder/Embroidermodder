/*!
 * \file emb_convert.c
 * \brief Utility for converting between and rendering machine embroidery file formats.
 *
 * Libembroidery 1.0.0-alpha
 * https://www.libembroidery.org
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright 2018-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Frontend for compiling as the command line tool embroider.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "embroidery.h"

/* same order as flag_list, to use in jump table */
#define FLAG_TO                        0
#define FLAG_TO_SHORT                  1
#define FLAG_HELP                      2
#define FLAG_HELP_SHORT                3
#define FLAG_FORMATS                   4
#define FLAG_FORMATS_SHORT             5
#define FLAG_QUIET                     6
#define FLAG_QUIET_SHORT               7
#define FLAG_VERBOSE                   8
#define FLAG_VERBOSE_SHORT             9
#define FLAG_VERSION                  10
#define FLAG_VERSION_SHORT            11
#define FLAG_RENDER                   12
#define FLAG_RENDER_SHORT             13
#define FLAG_SIMULATE                 14
#define FLAG_COMBINE                  15
#define FLAG_REPORT                   16
#define FLAG_REPORT_SHORT             17
#define NUM_FLAGS                     18

const char *help_msg[] = {
    "Usage: emb_convert [OPTIONS] fileToRead... ",
    "",
    "Conversion:",
    "    -t, --to        Convert all files given to the format specified by the arguments to",
    "                    the flag, for example:",
    "                        $ embroider -t dst input.pes",
    "                    would convert \"input.pes\" to \"input.dst\" in the same directory",
    "                    the program runs in.",
    "",
    "                    The accepted input formats are (TO BE DETERMINED).",
    "                    The accepted output formats are (TO BE DETERMINED).",
    "",
    "Analysis:",
    "    -R, --report    Report various statistics on the loaded pattern.",
    "",
    "Output:",
    "    -h, --help       Print this message.",
    "    -F, --formats    Print help on the formats that embroider can deal with.",
    "    -q, --quiet      Only print fatal errors.",
    "    -V, --verbose    Print everything that has reporting.",
    "    -v, --version    Print the version.",
    "",
    "Modify patterns:",
    "    --combine        takes 3 arguments and combines the first",
    "                     two by placing them atop each other and",
    "                     outputs to the third",
    "                        $ embroider --combine a.dst b.dst output.dst",
    "EOF"
};

const char *welcome_message = "emb_convert\n"
    "    A command line program for converting between machine embroidery file formats.\n"
    "    Copyright 2018-2024 The Embroidermodder Team\n"
    "    Licensed under the terms of the zlib license.\n"
    "\n"
    "    https://github.com/Embroidermodder/libembroidery\n"
    "    https://www.libembroidery.org\n";

/* EmbString table of flags. */
const EmbString flag_list[] = {
    "--to",
    "-t",
    "--help",
    "-h",
    "--formats",
    "-F",
    "--quiet",
    "-q",
    "--verbose",
    "-V",
    "--version",
    "-v",
    "--render",
    "-r",
    "--simulate",
    "--combine",
    "--report",
    "-R"
};

/*! TODO: Add capability for converting multiple files of various types to a
 * single format.
 * Currently, we only convert a single file to multiple formats.
 */

/*! Construct from tables above somehow, like how getopt_long works,
 * but we're avoiding that for compatibility
 * (C90, standard libraries only).
 */
void
usage(void)
{
	int i;
    puts(welcome_message);
    for (i=0; strcmp(help_msg[i], "EOF"); i++) {
        puts(help_msg[i]);
    }
}

/*! TODO: Add capability for converting multiple files of various types to a
 * single format. Currently, we only convert a single file to multiple formats.
 */
int
main(int argc, char *argv[])
{
    EmbPattern *current_pattern = emb_pattern_create();
    int i, j, result;
    /* If no argument is given, drop into the postscript interpreter. */
    if (argc == 1) {
        usage();
        return 0;
    }

    char *script = (char *)malloc(argc*100);
    int flags = argc - 1;
    for (i=1; i < argc; i++) {
        result = -1;
        /* identify what flag index the user may have entered */
        for (j=0; j < NUM_FLAGS; j++) {
            if (!strcmp(flag_list[j], argv[i])) {
                result = j;
                break;
            }
        }
        /* apply the flag */
        switch (result) {
        case FLAG_TO:
        case FLAG_TO_SHORT: {
            to_flag(argv, argc, i);
            break;
        }
        case FLAG_HELP:
        case FLAG_HELP_SHORT: {
            usage();
            break;
        }
        case FLAG_FORMATS:
        case FLAG_FORMATS_SHORT: {
            formats();
            break;
        }
        case FLAG_QUIET:
        case FLAG_QUIET_SHORT: {
            emb_verbose = -1;
            break;
        }
        case FLAG_VERBOSE:
        case FLAG_VERBOSE_SHORT: {
            emb_verbose = 1;
            break;
        }
        case FLAG_RENDER:
        case FLAG_RENDER_SHORT: {
            if (i + 2 < argc) {
                /* the user appears to have entered filenames after render */
                emb_pattern_readAuto(current_pattern, argv[i+1]);
                printf("%d\n", current_pattern->stitch_list->count);
                emb_pattern_render(current_pattern, argv[i+2]);
                i += 2;
                break;
                i++;
                if (argv[i][0] == '-') {
                    /* they haven't, use the default name */
                    puts("Defaulting to the output name 'output.png'.");
                    emb_pattern_render(current_pattern, "output.png");
                    i--;
                }
                else {
                    /* they have, use the user-supplied name */
                    emb_pattern_render(current_pattern, argv[i]);
                }
            }
            else {
                puts("Defaulting to the output name 'output.png'.");
                emb_pattern_render(current_pattern, "output.png");
            }
            break;
        }
        case FLAG_SIMULATE: {
            if (i + 1 < argc) {
                /* the user appears to have entered a filename after render */
                i++;
                if (argv[i][0] == '-') {
                    /* they haven't, use the default name */
                    puts("Defaulting to the output name 'output.avi'.");
                    emb_pattern_simulate(current_pattern, "output.avi");
                    i--;
                }
                else {
                    /* they have, use the user-supplied name */
                    emb_pattern_simulate(current_pattern, argv[i]);
                }
            }
            else {
                puts("Defaulting to the output name 'output.avi'.");
                emb_pattern_simulate(current_pattern, "output.avi");
            }
            break;
        }
        case FLAG_COMBINE: {
            if (i + 3 < argc) {
                EmbPattern *out;
                EmbPattern *p1 = emb_pattern_create();
                EmbPattern *p2 = emb_pattern_create();
                emb_pattern_readAuto(p1, argv[i+1]);
                emb_pattern_readAuto(p2, argv[i+2]);
                out = emb_pattern_combine(p1, p2);
                emb_pattern_writeAuto(out, argv[i+3]);
                emb_pattern_free(p1);
                emb_pattern_free(p2);
                emb_pattern_free(out);
            }
            else {
                puts("--combine takes 3 arguments and you have supplied <3.");
            }
            break;
        }
        case FLAG_VERSION:
        case FLAG_VERSION_SHORT: {
            puts(version_string);
            break;
        }
        case FLAG_REPORT:
        case FLAG_REPORT_SHORT: {
            emb_pattern_readAuto(current_pattern, argv[i+1]);
            emb_pattern_details(current_pattern);
            i++;
            break;
        }
        default:
            flags--;
            break;
        }
    }

    /* No flags set: default to simple from-to conversion. */
    if (!flags && argc == 3) {
        convert(argv[1], argv[2]);
    }
    else {
        if (!flags) {
            puts("Please enter an output format for your file, see --help.");
        }
    }
    emb_pattern_free(current_pattern);
    safe_free(script);
    return 0;
}

