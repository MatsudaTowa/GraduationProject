//================================
//
//sound.cppに必要な宣言[sound.h]
//Author:yuuto shimadu
// 
//================================

#ifndef _SOUND_H_ //このマクロ定義がされていなかったら
#define _SOUND_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"

//サウンドクラス
class CSound
{
public:
	//サウンドの列挙型
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		//タイトルBGM
		SOUND_LABEL_BGM_LOBBY,			//ロビーBGM
		SOUND_LABEL_BGM_DUEL,			//デュエルBGM
		SOUND_LABEL_BGM_RESULT,			//リザルトBGM
		SOUND_LABEL_MAX,				//最大値
	}SOUND_LABEL;

	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
		float Volume;			// 音量
	} SOUNDINFO;

	//定数
	static const SOUNDINFO SOUND_INFO[SOUND_LABEL_MAX];

	//メンバ関数
	CSound();								//コンストラクタ
	~CSound();								//デストラクタ
	HRESULT Init(HWND hWnd);				//初期化
	void Uninit(void);						//終了処理
	HRESULT PlaySound(SOUND_LABEL label);	//サウンドを流す
	void Stop(SOUND_LABEL label);			//サウンドを一つを止める
	void Stop(void);						//全てのサウンドを止める
private:
	IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
};

#endif