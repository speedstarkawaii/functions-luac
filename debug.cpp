//debug library

static int debug_getconstant(lua_State* L) {
    int index = luaL_checkinteger(L, 2);
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushstring(L, get_constant(func, index));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushstring(L, get_constant(level, index));
    }
    return 1;
}

static int debug_getconstants(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_constants(func));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_constants(level));
    }
    return 1;
}

static int debug_getinfo(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_info(func));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_info(level));
    }
    return 1;
}

static int debug_getproto(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_proto(func, luaL_checkinteger(L, 2)));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_proto(level, luaL_checkinteger(L, 2)));
    }
    return 1;
}

static int debug_getprotos(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_protos(func));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_protos(level));
    }
    return 1;
}

static int debug_getstack(lua_State* L) {
    int level = luaL_checkinteger(L, 1);
    if (lua_isnone(L, 2)) {
        lua_pushlightuserdata(L, get_stack(level));
    } else {
        lua_pushlightuserdata(L, get_stack(level, luaL_checkinteger(L, 2)));
    }
    return 1;
}

static int debug_getupvalue(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_upvalue(func, luaL_checkinteger(L, 2)));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_upvalue(level, luaL_checkinteger(L, 2)));
    }
    return 1;
}

static int debug_getupvalues(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        lua_pushlightuserdata(L, get_upvalues(func));
    } else {
        int level = luaL_checkinteger(L, 1);
        lua_pushlightuserdata(L, get_upvalues(level));
    }
    return 1;
}

static int debug_setconstant(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        set_constant(func, luaL_checkinteger(L, 2), luaL_checkstring(L, 3));
    } else {
        int level = luaL_checkinteger(L, 1);
        set_constant(level, luaL_checkinteger(L, 2), luaL_checkstring(L, 3));
    }
    return 0;
}

static int debug_setstack(lua_State* L) {
    int level = luaL_checkinteger(L, 1);
    set_stack(level, luaL_checkinteger(L, 2), luaL_checkany(L, 3));
    return 0;
}

static int debug_setupvalue(lua_State* L) {
    if (lua_type(L, 1) == LUA_TFUNCTION) {
        lua_CFunction func = lua_tocfunction(L, 1);
        set_upvalue(func, luaL_checkinteger(L, 2), luaL_checkany(L, 3));
    } else {
        int level = luaL_checkinteger(L, 1);
        set_upvalue(level, luaL_checkinteger(L, 2), luaL_checkany(L, 3));
    }
    return 0;
}
