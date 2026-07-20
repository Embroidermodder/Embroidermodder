#include <stdlib.h>
#include <inttypes.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "core.h"
#include "gui.h"

int get_str(const char *table, const char *key, char buffer[200]);

static lua_State *L;

int call(State *state, const char *cmd);

static int cmd(lua_State *L)
{
    const char *cmd_line = lua_tostring(L, -1);
    call(&state, cmd_line);
    return 0;
}

int script_env_init(void)
{
    printf("Creating lua state.\n");
    L = luaL_newstate();
    luaL_openlibs(L);

    printf("Loading c functions into FFI.\n");

    lua_pushcfunction(L, cmd);
    lua_setglobal(L, "cmd");

    printf("Calling boot script.\n");
    if (luaL_dofile(L, "./scripts/boot.lua") != LUA_OK) {
        return 0;
    }

    char buffer[200];
    get_str("test", "nested", buffer);
    printf("test.nested = %s\n", buffer);
    get_str("test", "other", buffer);
    printf("test.other = %s\n", buffer);
    get_str("test", "missing", buffer);

    return 1;
}

int get_str(const char *table, const char *key, char buffer[200])
{
    lua_getglobal(L, table);
    if (lua_istable(L, -1)) {
        lua_pushstring(L, key);
        lua_gettable(L, -2);
        if (!lua_isstring(L, -1)) {
            printf("ERROR: \"%s\" is not a string in the table \"%s\".\n",
                key, table);
        }
        const char *result = lua_tostring(L, -1);
        sprintf(buffer, "%s", result);
        lua_pop(L, 1);
    }
    else {
        printf("ERROR: \"%s\" is not a table in the lua state.\n",
            table);
    }
    return 1;
}

void script_run(const char *line)
{
    luaL_dostring(L, line);
}

void script_env_free(void)
{
    printf("Shutting down lua script environment.\n");
    lua_close(L);
}

