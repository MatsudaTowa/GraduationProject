#include <iostream>
#include <windows.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Luaスクリプトを読み込む
    if (luaL_dofile(L, "data\\test.lua") == 0) {
        // Lua変数 text を取得
        lua_getglobal(L, "text");

        if (lua_isstring(L, -1)) {
            const std::string str = lua_tostring(L, -1);
            std::cout << "C++側で取得: " << str << std::endl;
        }
    }
    else {
        // エラー出力
        const char* err = lua_tostring(L, -1);
        std::cerr << "Luaエラー: " << err << std::endl;
    }

    lua_close(L);
    return 0;
}