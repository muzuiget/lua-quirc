#include <lauxlib.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

typedef struct {
    char *bytes;
    unsigned long length;
    unsigned long width;
    unsigned long height;
} qrimageraw;

static int readfile(lua_State *L) {
    const char *filename = lua_tostring(L, 1);
    unsigned long width = lua_tonumber(L, 2);
    unsigned long height = lua_tonumber(L, 3);

    FILE *file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *bytes = (char *)malloc(length);
    fread(bytes, length, 1, file);
    fclose(file);

    qrimageraw *img = (qrimageraw *)malloc(sizeof(qrimageraw));
    img->bytes = bytes;
    img->length = length;
    img->width = width;
    img->height = height;
    lua_pushlightuserdata(L, img);
    return 1;
};

LUAMOD_API int luaopen_readraw(lua_State *L) {
    lua_pushcfunction(L, readfile);
    return 1;
}
