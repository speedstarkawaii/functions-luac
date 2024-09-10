//more impl
//theseneed another approach
//assuming u updated luau strucs vm shuffle etc
//need access to LS
//or works but etc


#include <lz4.h>
#include <string>

static int getnamecallmethod(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "__namecall");//or rawgetfield
    if (lua_isnil(L, -1)) {
        return luaL_error(L, "namecall method not setted");
    }
    return 1;
}

static int identifyexecutor(lua_State* L) {
    const std::string name = "skid";
    const std::string version = "1.0";//skid

    lua_pushstring(L, name.c_str());
    lua_pushstring(L, version.c_str());

    return 2;
}

static int lz4compress(lua_State* L) {
    size_t input_size;
    const char* input = luaL_checklstring(L, 1, &input_size);

    int max_compressed_size = LZ4_compressBound(input_size);
    char* compressed = new char[max_compressed_size];

    int compressed_size = LZ4_compress_default(input, compressed, static_cast<int>(input_size), max_compressed_size);

    if (compressed_size < 0) {
        delete[] compressed;
        return luaL_error(L, "compression failed");
    }

    lua_pushlstring(L, compressed, compressed_size);
    delete[] compressed;

    return 1;
}

static int lz4decompress(lua_State* L) {
    size_t compressed_size;
    const char* compressed = luaL_checklstring(L, 1, &compressed_size);
    size_t decompressed_size = luaL_checkinteger(L, 2);

    char* decompressed = new char[decompressed_size];

    int result = LZ4_decompress_safe(compressed, decompressed, static_cast<int>(compressed_size), static_cast<int>(decompressed_size));

    if (result < 0) {
        delete[] decompressed;
        return luaL_error(L, "decompression failed");
    }

    lua_pushlstring(L, decompressed, result);
    delete[] decompressed;

    return 1;
}

static int messagebox(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    const char* caption = luaL_checkstring(L, 2);
    int flags = luaL_checkinteger(L, 3);

    int result = MessageBox(NULL, text, caption, flags);//might need to check if codes are valid???

    lua_pushnumber(L, result);
    return 1;
}

static int setfpscap(lua_State* L) {
    int fps = luaL_checkinteger(L, 1);

    exit(fps);//haha make this urself uneed fps offset
    return 0;
}


//needs to fix
/*

struct HttpResponse {
    std::string body;
    long status_code;
    std::string status_message;
    bool success;
    std::unordered_map<std::string, std::string> headers;
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static int request(lua_State* L) {
    const char* url = luaL_checkstring(L, 1);
    const char* method = luaL_checkstring(L, 2);
    const char* body = luaL_optstring(L, 3, nullptr);
    
    luaL_checktype(L, 4, LUA_TTABLE); // headers
    luaL_checktype(L, 5, LUA_TTABLE); // cookies
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        return luaL_error(L, "failed to initilize curl");
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    struct curl_slist* headers = nullptr;
    lua_pushnil(L);
    while (lua_next(L, 4) != 0) {
        const char* key = lua_tostring(L, -2);
        const char* value = lua_tostring(L, -1);
        std::string header = std::string(key) + ": " + value;
        headers = curl_slist_append(headers, header.c_str());
        lua_pop(L, 1);
    }
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
    if (strcmp(method, "POST") == 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    } else if (strcmp(method, "PUT") == 0) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    } else if (strcmp(method, "PATCH") == 0) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
    
    std::string response_body;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
    
    CURLcode res = curl_easy_perform(curl);
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    HttpResponse response;
    response.body = response_body;
    response.status_code = response_code;
    response.status_message = curl_easy_strerror(res);
    response.success = (res == CURLE_OK);
    
    lua_newtable(L);
    lua_pushstring(L, response.body.c_str());
    lua_setfield(L, -2, "Body");
    lua_pushnumber(L, response.status_code);
    lua_setfield(L, -2, "StatusCode");
    lua_pushstring(L, response.status_message.c_str());
    lua_setfield(L, -2, "StatusMessage");
    lua_pushboolean(L, response.success);
    lua_setfield(L, -2, "Success");


    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    return 1;
}
*/

