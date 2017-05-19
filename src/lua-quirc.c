#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lauxlib.h>
#include <quirc.h>

typedef struct {
    char *bytes;
    unsigned long length;
    unsigned long width;
    unsigned long height;
} qrimageraw;

static void set_info_table(lua_State *L, struct quirc_data *qd) {
    lua_createtable(L, 0, 6);
    lua_pushinteger(L, qd->version);
    lua_setfield(L, -2, "version");
    lua_pushinteger(L, qd->ecc_level);
    lua_setfield(L, -2, "ecc_level");
    lua_pushinteger(L, qd->mask);
    lua_setfield(L, -2, "mask");
    lua_pushinteger(L, qd->data_type);
    lua_setfield(L, -2, "data_type");
    lua_pushinteger(L, qd->payload_len);
    lua_setfield(L, -2, "length");
    lua_pushlstring(L, (char *)qd->payload, qd->payload_len);
    lua_setfield(L, -2, "payload");
};

static int scan_image(const char *imgbytes, unsigned long imglen,
        unsigned long imgwidth, unsigned long imgheight,
        struct quirc_data *qd) {

    struct quirc *q = quirc_new();
    if (!q) {
        perror("Error: quirc_new()");
        return -1;
    }

    if (quirc_resize(q, imgwidth, imgheight) < 0) {
        perror("Error: quirc_resize()");
        return -2;
    }

    uint8_t *img = quirc_begin(q, NULL, NULL);
    memcpy(img, imgbytes, imglen);
    quirc_end(q);

    int id_count = quirc_count(q);
    if (id_count == 0) {
        fprintf(stderr, "Error: not a valid qrcode\n");
        return -3;
    }

    struct quirc_code code;
    quirc_extract(q, 0, &code);
    quirc_decode(&code, qd);

    quirc_destroy(q);
    return 0;
}

static int scan(lua_State *L) {
    unsigned long imglen = lua_rawlen(L, 1);
    const char *imgbytes = lua_tostring(L, 1);
    unsigned long imgwidth = lua_tonumber(L, 2);
    unsigned long imgheight = lua_tonumber(L, 3);

    if (imglen != (imgwidth * imgheight)) {
        fprintf(stderr, "Error: image length not match dimension\n");
        return 0;
    }

    struct quirc_data qd;
    int ret = scan_image(imgbytes, imglen, imgwidth, imgheight, &qd);
    if (ret < 0) {
        return 0;
    }

    set_info_table(L, &qd);
    return 1;
}

static int scan_lud(lua_State *L) {
    qrimageraw *img = (qrimageraw *)lua_touserdata(L, 1);

    if (img->length != (img->width * img->height)) {
        fprintf(stderr, "Error: image length not match dimension\n");
        return 0;
    }

    struct quirc_data qd;
    int ret = scan_image(img->bytes, img->length, img->width, img->height, &qd);
    if (ret < 0) {
        return 0;
    }

    set_info_table(L, &qd);
    return 1;
}

static const luaL_Reg quirclib[] = {
    {"scan", scan},
    {"scan_lud", scan_lud},
    {NULL, NULL},
};

LUAMOD_API int luaopen_quirc(lua_State *L) {
    luaL_newlib(L, quirclib);
    return 1;
}
