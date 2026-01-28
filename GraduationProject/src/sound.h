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
#include <functional>
#include <xaudio2.h>

// コールバッククラス
class CSoundCallBack : public IXAudio2VoiceCallback
{
public:
	std::function<void()>onFinish;
	void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext)override
	{
		if (onFinish)
		{
			onFinish();
		}
	}
	void STDMETHODCALLTYPE OnStreamEnd() override {} 
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {} 
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) override {} 
	void STDMETHODCALLTYPE OnBufferStart(void*) override {} 
	void STDMETHODCALLTYPE OnLoopEnd(void*) override {} 
	void STDMETHODCALLTYPE OnVoiceError(void*, HRESULT) override {}
};

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
		SOUND_LABEL_BGM_WINNER,			//勝者BGM
		SOUND_LABEL_BGM_LOSER,			//敗北者BGM
		SOUND_LABEL_SE_MOVELOBBY,		//ロビー遷移
		SOUND_LABEL_SE_INLOBBY,			//ロビー入出
		SOUND_LABEL_SE_READY,			//準備完了音
		SOUND_LABEL_SE_CARD_SHUFFL,		//カードシャッフル
		SOUND_LABEL_SE_COUNT_DOWN,		//カウントダウン
		SOUND_LABEL_SE_DUEL,			//デュエル
		SOUND_LABEL_SE_CARD_DRAW,		//カードドロー
		SOUND_LABEL_SE_CAST_FAIL,		//キャスト失敗
		SOUND_LABEL_SE_CAST_CANCEL,		//キャストキャンセル
		SOUND_LABEL_SE_RANKING,			//ランキング出現
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
	HRESULT PlaySound(SOUND_LABEL label, std::function<void()>onFinish);
	void Stop(SOUND_LABEL label);			//サウンドを一つを止める
	void Stop(void);						//全てのサウンドを止める
	bool IsPlaySound(SOUND_LABEL label);	//音が鳴り終わったか判断

	void LoadDuelBGMList();	// 戦闘用BGM読み込み処理

	HRESULT PlaySoundFromFile(const char* filename, float volume, int loopCount);
private:
	IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
	WAVEFORMATEX m_aWfx[SOUND_LABEL_MAX];
	std::vector<std::string>m_DuelBGMList;
	IXAudio2SourceVoice* m_pDuelBGMVoice = nullptr;	// デュエルBGM専用のソースボイス
};

#endif