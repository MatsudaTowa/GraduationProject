//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "../../system/main.h"
#include "xaudio2.h"

namespace My
{

	class CSound
	{
	public:
		//*****************************************************************************
		// サウンド一覧
		//*****************************************************************************
		typedef enum
		{
			SOUND_TITLE_000 = 0,		// タイトル音楽
			SOUND_STAGE_SELECT_000,	// リザルト
			SOUND_STAGE_00,			// ステージ000
			SE_CHOICE_000,		// 選択
			SE_DECISION_000,	// 決定
			SE_FOOTSTEPS_000,	// 足音
			SE_GET_ITEM_000,	// 足音
			SOUND_LABEL_MAX,
		} SOUND_LABEL;
		//*****************************************************************************
		// サウンド情報の構造体定義
		//*****************************************************************************
		typedef struct
		{
			const char* pFilename;	// ファイル名
			int nCntLoop;		// ループカウント
		} SOUNDINFO;
		CSound();
		~CSound();
		HRESULT InitSound(HWND hWnd);		// 初期化処理
		void UninitSound(void);				// 終了処理
		HRESULT PlaySound(SOUND_LABEL label);	// 再生
		void StopSound(SOUND_LABEL label);		// 停止
		void StopSound(void);					// 停止(全て)
		bool IsPlaySound(SOUND_LABEL label);	// 再生状態取得
	protected:
		HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
		HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	private:
		IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
		IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
		IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
		BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
		DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
		// サウンドの情報
		SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX];
	};
}
#endif