#include <iostream>
#include <windows.h>
#include <conio.h>  // _kbhit, _getch 用
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

std::string UTF8ToSJIS(const std::string& utf8);

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Luaスクリプト読み込み
    if (luaL_dofile(L, "data/test.lua") != LUA_OK) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        return -1;
    }

    // Lua関数をスタックに積む
    lua_getglobal(L, "generateText"); // ここで generateText をスタックに置く

    // 引数0個、戻り値1個で呼ぶ
    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        return -1;
    }

    // Luaの戻り値を取得
    const std::string utf8Text = lua_tostring(L, -1);
    const std::string sjisText = UTF8ToSJIS(utf8Text);
    std::cout << "Lua生成文字列: " << sjisText << std::endl;

    lua_close(L);
    return 0;
}

std::string UTF8ToSJIS(const std::string& utf8)
{
    // UTF-8 → UTF-16
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
    std::wstring wide(wide_len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wide[0], wide_len);

    // UTF-16 → Shift_JIS
    int sjis_len = WideCharToMultiByte(932, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string sjis(sjis_len, 0);
    WideCharToMultiByte(932, 0, wide.c_str(), -1, &sjis[0], sjis_len, nullptr, nullptr);

    return sjis;
}