/* Command: test
 *
 * NOTE: Test is necessarily a compound command so it doesn't call init or end.
 */

#include "commands.h"

#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

const char *test_script[] = {
    "new",
    "sleep",
    "panup",
    "panup",
    "panleft",
    "panright",
    "pandown",
    "zoomin",
    "zoomout",
    "day",
    "night",
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "icon32",
    "about",
    "^END^"
};

int test_cmd(State *state)
{
/*
    int wait = 200;
    int i;
    for (i=0; test_script[i][0] != '^'; i++) {
        call(state, test_script[i]);
        repaint();
#if WINDOWS
        Sleep(wait);
#else
        usleep(wait*1000);
#endif
    }
*/
    script_env_call("script/test.py");
    return 0;
}

