#include <lua.hpp>
//use local bit = bit32 or ha0a

// bit.band
static int bit_band(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a & b);
    return 1;
}

// bit.bor
static int bit_bor(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a | b);
    return 1;
}

// bit.bxor
static int bit_bxor(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a ^ b);
    return 1;
}

// bit.bnot
static int bit_bnot(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    lua_pushinteger(L, ~a);
    return 1;
}

// bit.lshift
static int bit_lshift(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int shift = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a << shift);
    return 1;
}

// bit.rshift
static int bit_rshift(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int shift = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a >> shift);
    return 1;
}

// bit.rol
static int bit_rol(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int shift = luaL_checkinteger(L, 2);
    int bits = sizeof(a) * 8;
    lua_pushinteger(L, (a << shift) | (a >> (bits - shift)));
    return 1;
}

// bit.ror
static int bit_ror(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int shift = luaL_checkinteger(L, 2);
    int bits = sizeof(a) * 8;
    lua_pushinteger(L, (a >> shift) | (a << (bits - shift)));
    return 1;
}
