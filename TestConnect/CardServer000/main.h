//================================
//
// Server用のヘッダー[main.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _MAIN_H_
#define _MAIN_H_

//インクルード
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment (lib, "ws2_32.lib")

//Json読み込み用
#include <json.hpp>
#include <json_fwd.hpp>

using ordered_json = nlohmann::ordered_json;

#endif // !_MAIN_H_