#include <iostream>
#include <windows.h>
#include <conio.h>  // _kbhit, _getch �p
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Lua�X�N���v�g��ǂݍ���
    if (luaL_dofile(L, "data\\test.lua") == 0) {
        // Lua�ϐ� text ���擾
        lua_getglobal(L, "text");

        if (lua_isstring(L, -1)) {
            const std::string str = lua_tostring(L, -1);
            std::cout << "C++���Ŏ擾: " << str << std::endl;
        }
    }
    else {
        // �G���[�o��
        const char* err = lua_tostring(L, -1);
        std::cerr << "Lua�G���[: " << err << std::endl;
    }

    lua_close(L);

    std::cout << "ESC�L�[�ŏI�����܂��B" << std::endl;
    // ���C�����[�v�iESC�����܂ő����j
    while (true)
    {
        if (_kbhit())  // �L�[�������ꂽ��
        {
            int key = _getch();
            if (key == 27)  // 27 �� ESC ��ASCII�R�[�h
            {
                std::cout << "ESC�L�[�������ꂽ�̂ŏI�����܂��B" << std::endl;
                break;
            }
        }
    }
    return 0;
}