//================================
//
// TCP接続のサーバー用のヘッダー[tcp_client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _RAKNET_SERVER_H_
#define _RAKNET_SERVER_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "raknet_data.h"

//RakNetサーバーのクラス
class CDuel_Manager
{
public:

	//対戦時に決められるルールの構造体
	struct DuelRuleParam
	{
		int nDuelTime;			//対戦時間
		int nNeedEnergyUpTime;	//エナジーの増加に必要な時間
		int nCardStayTime;		//カードステイ時間
		int nStartLife;			//開始時の体力
	};

	//関数
	~CDuel_Manager() {}							//デストラクタ
	
	//シングルトン
	static CDuel_Manager* GetInstance()
	{
		static CDuel_Manager instance; //静的インスタンス
		return &instance;
	}

private:

	//関数
	CDuel_Manager() : m_DuelRuleParam() {}		//コンストラクタ

	//変数
	DuelRuleParam m_DuelRuleParam;				//対戦のルールパラメータ
};

#endif