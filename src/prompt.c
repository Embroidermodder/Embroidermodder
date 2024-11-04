/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Prompt
 */

#include <stdio.h>

#define MAX_HISTORY            300

#include "core.h"

EmbString prompt_input_list[MAX_HISTORY];
int prompt_history_size = 0;
int prompt_history_position = 0;

/* . */
void
prompt_history_appended(EmbString txt)
{
    sprintf(promptHistoryData, "%s<br/>%s", promptHistoryData, txt);
}

/* . */
void
log_prompt_input(EmbString txt)
{
    string_copy(prompt_input_list[prompt_history_size], txt);
    prompt_history_size++;
    prompt_history_position = prompt_history_size;
}

/* . */
void
prompt_input_previous(void)
{
    if (prompt_history_size <= 0) {
        critical_box(translate("Prompt Previous Error"),
            translate("The prompt input is empty! Please report this as a bug!"));
        debug_message("The prompt input is empty! Please report this as a bug!");
    }
    else {
        prompt_history_position--;
        int maxNum = prompt_history_size;
        if (prompt_history_position < 0) {
            prompt_history_position = 0;
            prompt_set_current_text("");
        }
        else if (prompt_history_position >= maxNum) {
            prompt_history_position = maxNum;
            prompt_set_current_text("");
        }
        else {
            prompt_set_current_text(prompt_input_list[prompt_history_position]);
        }
    }
}

/* . */
void
prompt_input_next(void)
{
    if (prompt_history_size <= 0) {
        critical_box(translate("Prompt Next Error"),
            translate("The prompt input is empty! Please report this as a bug!"));
        debug_message("The prompt input is empty! Please report this as a bug!");
    }
    else {
        prompt_history_position++;
        int maxNum = prompt_history_size;
        if (prompt_history_position < 0) {
            prompt_history_position = 0;
            prompt_set_current_text("");
        }
        else if (prompt_history_position >= maxNum) {
            prompt_history_position = maxNum;
            prompt_set_current_text("");
        }
        else {
            prompt_set_current_text(prompt_input_list[prompt_history_position]);
        }
    }
}

