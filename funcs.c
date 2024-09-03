//be internal for this
//MAY need additional offsets unlesss u update luau
//MUST BE REGISTERED
int rbx_luastate;

int rbx_top = 0;
int rbx_base = 0;

int getrawmetatable(lua_State* L) {
    if (lua_gettop(L) != 1) {
        return rbx_top//whoopsies
    }
    if (!lua_getmetatable(L, 1)) {
        lua_pushnil(L);
        return 1;
    }
    return 1;
}

int setrawmetatable(lua_State* L) {
    if (lua_gettop(L) != 2) {
        return luaL_error(L, " NOOB YOU DO 2 ARGS!");//error
    }
    if (!lua_istable(L, 2) && !lua_isnil(L, 2)) {
        return luaL_error(L, " argument must be a table or nil");//error
    }
    lua_setmetatable(L, 1);
    lua_pushboolean(L, 1);
    return 1;
}

int PUBLICREADONLY = false;//ignore was for scripts u can disable
int setreadonly(lua_State* L) {
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "Expected 2 args noob");
    }
    if (!lua_istable(L, 1)) {
        return luaL_error(L, "First argument got to be table");
    }
    if (!lua_isboolean(L, 2)) {
        return luaL_error(L, "Second argument must be a BOOL kid");
    }

    bool readonly = lua_toboolean(L, 2);

    if (readonly) {
        lua_pushvalue(L, 1);
        lua_createtable(L, 0, 1);
        lua_pushboolean(L, 1);
        lua_setfield(L, -2, "__metatable");
        lua_setmetatable(L, -2);
       PUBLICREADONLY = 1;
    } else {
        lua_pushnil(L);
        lua_setmetatable(L, 1);
    }

    return 0;
}

int isreadonly(lua_State* L) {
    if (lua_gettop(L) != 1) {
        return luaL_error(L, " bruv 1 argument");
    }
    if (!lua_istable(L, 1)) {
        return luaL_error(L, "a must be a table");
    }

    if (lua_getmetatable(L, 1)) {
        lua_getfield(L, -1, "__metatable");
        if (lua_isboolean(L, -1)) {
            lua_pushboolean(L, lua_toboolean(L, -1));
            return 1;
        }
    }//

    lua_pushboolean(L, 0);
    return 1;
}

int getreg(lua_State* L) {
    lua_pushvalue(L, LUA_REGISTRYINDEX);
    return 1;
}

int getgc(lua_State* L) {
    lua_newtable(L);
    int index = 1;
    lua_gc(L, LUA_GCCOLLECT, 0);
    lua_gc(L, LUA_GCCOUNT, 0);

    lua_pushnil(L); //or i push some else
    while (lua_next(L, LUA_REGISTRYINDEX) != 0) {
        lua_pushvalue(L, -2); //getkey
        lua_seti(L, -4, index++);
        lua_pop(L, 1);  
    }

    return 1;
}

int firesignal(lua_State* L) {
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "Expected exactly 1 argument");
    }
    
    if (!lua_isuserdata(L, 1)) {
        return luaL_error(L, "Expected aa (RBXScriptSignal) as the argument");
    }

    void* signal = lua_touserdata(L, 1);

    if (signal) {
        MessageBox(NULL, "nope u gotta write this urself", 0x0, NULL);
        //nope u do this urself
    }

    return 0;
}

//MOUSE LIBS
//can use vim in rbx services
int isrbxactive(lua_State* L) {
    HWND rbx = FindWindow(NULL, "Roblox");//this will always be false if u directly exec from ur ui
    lua_pushboolean(L, GetForegroundWindow() == rbx);
    return 1;
}

int keypress(lua_State* L) {
    int keycode = luaL_checkinteger(L, 1);
    keybd_event((BYTE)keycode, 0, 0, 0);
    return 0;
}

int keyrelease(lua_State* L) {
    int keycode = luaL_checkinteger(L, 1);
    keybd_event((BYTE)keycode, 0, KEYEVENTF_KEYUP, 0);
    return 0;
}

int mouse1click(lua_State* L) {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    return 0;
}

int mouse1press(lua_State* L) {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    return 0;
}

int mouse1release(lua_State* L) {
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    return 0;
}

int mouse2click(lua_State* L) {
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    return 0;
}

int mouse2press(lua_State* L) {
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    return 0;
}

int mouse2release(lua_State* L) {
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    return 0;
}

int mousescroll(lua_State* L) {
    int px = luaL_checkinteger(L, 1);
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, px, 0);
    return 0;
}

int mousemoverel(lua_State* L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
    return 0;
}

int mousemoveabs(lua_State* L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    SetCursorPos(x, y);
    return 0;
}

std::unordered_map<lua_State*, lua_CFunction> original_functions;//diffeent approach for rbx thiswill work on old rbx i think

int hookfunction(lua_State* L) {
  //must be 2 types of funcs NOT string or table
    luaL_checktype(L, 1, LUA_TFUNCTION);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    lua_pushvalue(L, 1);
    lua_insert(L, -2);
    lua_pushcclosure(L, lua_tocfunction(L, 2), 0);
    lua_replace(L, -3);

    original_functions[L] = lua_tocfunction(L, 1);
    return 1;
}

int newcclosure(lua_State* L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushcclosure(L, lua_tocfunction(L, 1), 0);
    return 1;
}


int checkcaller(lua_State* L) {
    lua_pushboolean(L, true); //always return 1 or true
    return 1;
}

int islclosure(lua_State* L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushboolean(L, lua_isfunction(L, 1) && lua_iscclosure(L, 1) == 0);
    return 1;
}

int iscclosure(lua_State* L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);//func
    lua_pushboolean(L, lua_iscclosure(L, 1));
    return 1;
}


int dumpstring(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* script = lua_tostring(L, 1);
    
   //HAHA UR TURN :D

    return 1; 
}



void colorpush(const std::string& color) {//pro parser
    if (color == "@@BLACK@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
    else if (color == "@@BLUE@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    else if (color == "@@GREEN@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    else if (color == "@@CYAN@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    else if (color == "@@RED@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    else if (color == "@@MAGENTA@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
    else if (color == "@@BROWN@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    else if (color == "@@LIGHT_GRAY@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    else if (color == "@@DARK_GRAY@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    else if (color == "@@LIGHT_BLUE@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    else if (color == "@@LIGHT_GREEN@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    else if (color == "@@LIGHT_CYAN@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    else if (color == "@@LIGHT_RED@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    else if (color == "@@LIGHT_MAGENTA@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    else if (color == "@@YELLOW@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    else if (color == "@@WHITE@@") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

int rconsoleprint(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* message = lua_tostring(L, 1);
    std::string msg_str(message);

    size_t color_pos = msg_str.find("@@");
    if (color_pos != std::string::npos) {
        std::string color_code = msg_str.substr(color_pos, msg_str.find(" ", color_pos) - color_pos);
        colorpush(color_code);
        msg_str.erase(color_pos, color_code.length());
    }

    std::cout << msg_str << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); 
    return 0;
}

int rconsoleinfo(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* message = lua_tostring(L, 1);
    rconsoleprint(L); // Print the message
    std::cout << "[INFO] " << message << std::endl;
    return 0;
}

int rconsolewarn(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* message = lua_tostring(L, 1);
    rconsoleprint(L);
    std::cout << "[WARNING] " << message << std::endl;
    return 0;
}

int rconsoleerr(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* message = lua_tostring(L, 1);
    rconsoleprint(L); 
    std::cout << "[ERROR] " << message << std::endl;//custom error msg
    return 0;
}

int rconsoleclear(lua_State* L) {
    system("cls");
    return 0;
}

int rconsolename(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* title = lua_tostring(L, 1);
    SetConsoleTitle(title);
    return 0;
}

int rconsoleinput(lua_State* L) {
    std::string input;
    std::getline(std::cin, input);
    lua_pushstring(L, input.c_str());
    return 1;
}

int rconsoleinputasync(lua_State* L) {
    return rconsoleinput(L); //macro
}

bool nonoextens(const std::string& path) {
    std::vector<std::string> disallowedExtensions = {
        ".exe", ".scr", ".bat", ".com", ".csh", ".msi",
        ".vb", ".vbs", ".vbe", ".ws", ".wsf", ".wsh",
        ".ps1", ".psy"
    };

    for (const auto& ext : disallowedExtensions) {
        if (path.ends_with(ext)) {
            return false;
        }
    }
    return true;
}

int readfile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    std::ifstream file(path);
    
    if (!file.is_open()) {
        return luaL_error(L, "File does not exist.");
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    lua_pushstring(L, content.c_str());
    return 1;
}

int writefile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);
    const char* filepath = lua_tostring(L, 1);
    const char* contents = lua_tostring(L, 2);

    if (!nonoextens(filepath)) {
        return luaL_error(L, "Disallowed file extension.");
    }

    std::ofstream file(filepath);
    file << contents;
    file.close();
    return 0;
}

int appendfile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    const char* content = lua_tostring(L, 2);

    std::ofstream file(path, std::ios_base::app);
    
    if (!file.is_open()) {
        return luaL_error(L, "File does not exist.");
    }

    file << content;
    file.close();
    return 0;
}

int loadfile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    
    std::ifstream file(path);
    
    if (!file.is_open()) {
        lua_pushnil(L);
        lua_pushstring(L, "File does not exist.");
        return 2;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    lua_pushstring(L, content.c_str());
    return 1;
}

int listfiles(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* folder = lua_tostring(L, 1);
    
    lua_newtable(L);
    int index = 1;

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            lua_pushstring(L, entry.path().string().c_str());
            lua_rawseti(L, -2, index++);
        }
    }
    
    return 1; 
}

int isfile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    lua_pushboolean(L, std::filesystem::is_regular_file(path));
    return 1;
}

int isfolder(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    lua_pushboolean(L, std::filesystem::is_directory(path));
    return 1;
}

int makefolder(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* filepath = lua_tostring(L, 1);
    
    if (!std::filesystem::create_directory(filepath)) {
        return luaL_error(L, "Folder already exists or an error occurred.");
    }
    return 0;
}

int delfolder(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    
    if (!std::filesystem::remove(path)) {
        return luaL_error(L, "Folder does not exist or could not be deleted.");
    }
    return 0;
}

int delfile(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* path = lua_tostring(L, 1);
    
    if (!std::filesystem::remove(path)) {
        return luaL_error(L, "File does not exist or could not be deleted.");
    }
    return 0;
}

int setclipboard(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    const char* clipping = lua_tostring(L, 1);
    
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, strlen(clipping) + 1);
        if (hGlobal) {
            memcpy(GlobalLock(hGlobal), clipping, strlen(clipping) + 1);
            GlobalUnlock(hGlobal);
            SetClipboardData(CF_TEXT, hGlobal);
        }
        CloseClipboard();
    }
    
    return 0;
}


int messagebox(lua_State* L) {//might defer
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);
    luaL_checktype(L, 3, LUA_TNUMBER);
    
    const char* text = lua_tostring(L, 1);
    const char* caption = lua_tostring(L, 2);
    int style = lua_tointeger(L, 3);
    
    int result = MessageBoxA(NULL, text, caption, style);
    lua_pushinteger(L, result);
    
    return 1;
}

//may not work
int hookmetamethod(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checkstring(L, 2);
    luaL_checktype(L, 3, LUA_TFUNCTION);

    lua_pushvalue(L, 1);
    lua_gettable(L, LUA_REGISTRYINDEX);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        lua_pushnil(L);
        return 1;
    }

    lua_pushstring(L, lua_tostring(L, 2));
    lua_gettable(L, -2);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        lua_pushnil(L);
        return 1;
    }

    lua_pushvalue(L, 3);
    lua_pushvalue(L, 4);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, 1);
    lua_pushstring(L, lua_tostring(L, 2));
    lua_pushvalue(L, 3);
    lua_settable(L, -3);

    lua_pushvalue(L, 4);
    return 1;
}
