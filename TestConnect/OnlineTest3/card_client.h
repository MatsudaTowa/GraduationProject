//================================
//
// カードクライアント用のヘッダー[card_client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CARD_CLIENT_H_
#define _CARD_CLIENT_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "raknet_data.h"

//カードクライアントのクラス
class CCard_Client
{
public:

	enum RARITY
	{// レアリティの種類
		NONE_RARITY,
		COMMON,			// コモン
		UNCOMMON,		// アンコモン
		RARE,			// レア
		SUPERRARE,		// スーパーレア
		HYPERRARE,		// ハイパーレア
		ROYALRARE,		// ロイヤルレア
		EMPERORRARE,	// エンペラーレア
		GODRARE,		// ゴッドレア
		XRARE,			// エックスレア
	};

	enum CardType
	{// カードの種類
		NONE,
		ATTACK,
		DEFENSE,
		ASSIST,
	};

	enum AttackType
	{// 攻撃の種類
		NONE_ATTACK,			// 攻撃以外のときはこれ
		ALL_ATTACK,				// 全体攻撃
		SPECIFIC_ATTACK,		// 特定の相手を選んで攻撃
		RANDOM_ATTACK,			// ランダム攻撃
		SELFINTARGET_ATTACK,	// 自分を含めた攻撃
	};

	enum DefenseType
	{// 守備の種類
		NONE_DEFENSE,		// 守備以外のときはこれ
		COUNTER,			// 反撃できる
		NOT_COUNTER,		// 反撃できない
	};

	enum AssistType
	{// アシストの種類
		NONE_ASSIST,		// アシスト以外のときはこれ
		OBSTRUCT,			// 妨害
		BUFF,				// バフ
		DEBUFF				// デバフ
	};

	enum BuffType
	{// バフの種類
		NONE_BUFF,
		HEAL,				// HP回復
		NO_NAMEONE,			// 未定①
		NO_NAMETWO,			// 未定②
	};

	enum DeBuffType
	{// デバフの種類
		NONE_DEBUFF,
		DEATH,				// 死亡
	};

	enum HealType
	{// 回復の種類
		NONE_HEAL,					// 回復しない
		ALL_HEAL,					// 全体回復
		SPECIFIC_HEAL,				// 特定の相手を選んで回復
		RANDOM_HEAL,				// ランダム回復
		SELFINTARGET_HEAL,			// 自分を含めてランダム回復
		ONLY_ME,					// 自分だけ
	};

	//基本カードのパラメータ
	struct BaseParam
	{
		int nPackID;							// パック番号
		int nCardID;							// カード番号
		std::string Name;						// カード名
		std::string Ruby;						// フリガナ
		int nCost = 0;							// カードコスト
		CardType Maintype;						// カードの種類
		RARITY Raritytype;						// レアリティの種類
		std::string ImagePath;					// 画像ファイルパス
	};

	//アタックカードのパラメータ
	struct AttackParam
	{
		BaseParam BaseParam = {};	// 基本パラメータ
		int nDamage = 0;			// ダメージ数
		AttackType Attacktype;		// 攻撃の種類
	};

	//ディフェンスカードのパラメータ
	struct DefenseParam
	{
		BaseParam BaseParam = {};		// 基本パラメータ
		int nGuard = 0;					// ガード値
		int nCounter = 0;				// 反撃値
		DefenseType Defensetype;		// 守備の種類
	};

	//アシストカードのパラメータ
	struct AssistParam
	{
		BaseParam BaseParam = {};		// 基本パラメータ
		int nHeal = 0;					// 回復値
		BuffType Bufftype;				// バフの種類
		HealType Healtype;				// 回復の種類
		AssistType Assisttype;			// アシストの種類
		bool bIsOneTime;				// カード効果の発動時間(true = 単発,false = 単発じゃない)
		int nTime;						// 単発じゃないときの発動時間
	};

	// カード特有のパラメータの構造体まとめ
	struct CardParam
	{
		AttackParam attackParam;
		DefenseParam defenseParam;
		AssistParam assistParam;
	};

	//パラメータの構造体
	struct Param
	{
		int nPackID;							// パック番号
		int nCardID;							// カード番号
		std::string Name;						// カード名
		std::string Ruby;						// フリガナ
		int nCost = 0;							// カードコスト
		int nDamage = 0;						// ダメージ数
		int nGuard = 0;							// ガード値
		int nCounter = 0;						// 反撃値
		int nHeal = 0;							// 回復値
		CardType Maintype;						// カードの種類
		RARITY Raritytype;						// レアリティの種類
		AssistType Assisttype;					// アシストの種類
		AttackType Attacktype;					// 攻撃の種類
		DefenseType Defensetype;				// 守備の種類
		BuffType Bufftype;						// バフの種類
		HealType Healtype;						// 回復の種類
		std::string ImagePath;					// 画像ファイルパス
		CardParam Cardparam;					// カード特有のパラメータ種類
	};

	//列挙
	enum GAME_MESSAGE
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CARD_MESSAGE_1,			//カード情報の送信
		ID_ALLCARD_MESSAGE_1,		//全カードの情報を送信
	};

	//インスタンス
	static CCard_Client* GetInstance()
	{
		static CCard_Client instance; //静的インスタンス
		return &instance;
	}

	//関数
	~CCard_Client();											//デストラクタ
	bool Init();												//初期化処理
	void Uninit();												//終了処理

	//通信
	void Communication();										//通信処理
	void RequestCard(int id);									//カードリクエスト
	void RequestAllCard();										//全カードリクエスト
	void ReceiveCardInfo(RakNet::Packet* packet);				//カード情報の受信
	void ReceiveAllCardInfo(RakNet::Packet* packet);			//全カード情報の受信
	
private:

	//定数
	static constexpr int PORT{ 22334 };	//ポート番号

	//関数
	CCard_Client();														//コンストラクタ
	void Send(RakNet::RakPeerInterface* peer, RakNet::BitStream* out);	//送信処理
	void RegistCard(Param param);										//カードの登録

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	RakNet::RakPeerInterface* m_pPeer;	//ピア(接続用)
};

#endif