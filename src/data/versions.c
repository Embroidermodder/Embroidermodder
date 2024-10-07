/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Versions
 */

char *embroidermodder_version = "2.0.0-alpha4";
char *libembroidery_version = "1.0.0-alpha";
char *EmbroideryMobile_version = "1.0.0-alpha";
char *PET_version = "1.0.0-alpha";

/* TODO: find a source for compiler flags for operating system detection.
 */
#if defined(_WIN32)
const char *os = "Windows";
#elif defined(__CYGWIN__)
const char *os = "Cygwin";
#elif defined(__linux__)
const char *os = "Linux";
#elif defined(__unix__)
const char *os = "Unix";
#elif defined(__APPLE__) || defined(__MACH__)
const char *os = "Mac OS";
#elif defined(__FREEBSD__)
const char *os = "FreeBSD";
#elif defined(__ANDROID__)
const char *os = "Android";
#else
const char *os = "Unknown Operating System";
#endif

