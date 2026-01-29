//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

// サウンドの情報
const CSound::SOUNDINFO CSound::SOUND_INFO[CSound::SOUND_LABEL_MAX] =
{// SEの場合は第2引数を0にする第3引数は0～1.0f
	{"data\\SOUND\\BGM\\title000.wav", -1, 0.5f},			//タイトルBGM
	{"data\\SOUND\\BGM\\lobby000.wav", -1, 0.5f},			//ロビーBGM
	{"data\\SOUND\\BGM\\duel\\duel000.wav", -1, 0.5f},		//デュエルBGM
	{"data\\SOUND\\BGM\\winner.wav", -1, 0.5f},				//勝者BGM
	{"data\\SOUND\\BGM\\loser.wav", -1, 0.5f},				//リザルトBGM
	{"data\\SOUND\\SE\\movelobby.wav", 0, 1.0f},			//ロビー入出SE
	{"data\\SOUND\\SE\\inlobby000.wav", 0, 1.0f},			//ロビー入出SE
	{"data\\SOUND\\SE\\lobbyready000.wav", 0, 1.0f},		//準備完了SE
	{"data\\SOUND\\SE\\shuffling_cards.wav", 0, 1.0f},		//カードシャッフルSE
	{"data\\SOUND\\SE\\countdown.wav", 0, 1.0f},			//カウントダウンSE
	{"data\\SOUND\\SE\\duel000.wav", 0, 0.8f},				//デュエルSE
	{"data\\SOUND\\SE\\Draw_a_card.wav", 0, 1.0f},			//カードドローSE
	{"data\\SOUND\\SE\\card_cast.wav", 0, 1.0f},			//カードキャストSE
	{"data\\SOUND\\SE\\wait_shield.wav", 0, 1.0f},			//守備カード設置SE
	{"data\\SOUND\\SE\\Incorrect.wav", 0, 1.0f},			//キャスト失敗SE
	{"data\\SOUND\\SE\\Cutting_through.wav", 0, 1.0f},		//キャストキャンセルSE
	{"data\\SOUND\\SE\\damage.wav", 0, 1.0f},				//ダメージSE
	{"data\\SOUND\\SE\\guard.wav", 0, 1.0f},				//ガードSE
	{"data\\SOUND\\SE\\appear.wav", 0, 1.0f},				//ランキングSE
	{"data\\SOUND\\SE\\win.wav", 0, 1.0f},					//勝利SE
	{"data\\SOUND\\SE\\lose.wav", 0, 1.0f},					//敗北SE
	{"data\\SOUND\\SE\\click.wav", 0, 1.0f},				//クリックしたときのSE
	
};

//=============================================================================
// コンストラクタ
//=============================================================================
CSound::CSound() : 
	m_pXAudio2(nullptr),
	m_pMasteringVoice(nullptr)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(SOUND_INFO[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = SOUND_INFO[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		m_aWfx[nCntSound] = wfx.Format;
		// ファイルをクローズ
		CloseHandle(hFile);
	}

	LoadDuelBGMList();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	//XAUDIO2_VOICE_STATE xa2state;
	//XAUDIO2_BUFFER buffer;

	//// バッファの値設定
	//memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	//buffer.AudioBytes = m_aSizeAudio[label];
	//buffer.pAudioData = m_apDataAudio[label];
	//buffer.Flags = XAUDIO2_END_OF_STREAM;
	//buffer.LoopCount = SOUND_INFO[label].nCntLoop;

	//// 状態取得
	//m_apSourceVoice[label]->GetState(&xa2state);

	//// 音量取得
	//m_apSourceVoice[label]->SetVolume(SOUND_INFO[label].Volume);

	//if (xa2state.BuffersQueued != 0)
	//{// 再生中
	//	// 一時停止
	//	m_apSourceVoice[label]->Stop(0);

	//	// オーディオバッファの削除
	//	m_apSourceVoice[label]->FlushSourceBuffers();
	//}

	//// オーディオバッファの登録
	//m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	//// 再生
	//m_apSourceVoice[label]->Start(0);

	//return S_OK;

	if (label == SOUND_LABEL_BGM_DUEL)
	{// デュエルBGMを鳴らしたいときだけ鳴らすものを抽選
		if (!m_DuelBGMList.empty())
		{
			int index = IntRandom(0, m_DuelBGMList.size() - 1);
			const char* filename = m_DuelBGMList[index].c_str();

			// ランダムで選ばれたファイルを再生
			return PlaySoundFromFile(filename, SOUND_INFO[label].Volume, SOUND_INFO[label].nCntLoop);
		}
	}
	// SEの複数再生用
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = SOUND_INFO[label].nCntLoop;

	if (label == SOUND_LABEL_BGM_DUEL)
	{
		if (!m_DuelBGMList.empty())
		{
			int index = IntRandom(0, m_DuelBGMList.size());
			const char* filename = m_DuelBGMList[index].c_str();

		}
	}

	if (SOUND_INFO[label].nCntLoop == 0)
	{// SEの場合毎回SourceVoiceを作る
		IXAudio2SourceVoice* pVoice = nullptr;

		// 新規SourceVoice
		HRESULT hr = m_pXAudio2->CreateSourceVoice(&pVoice, &m_aWfx[label]);
		if (FAILED(hr))
		{
			return hr;
		}
		pVoice->SetVolume(SOUND_INFO[label].Volume);
		pVoice->SubmitSourceBuffer(&buffer);
		pVoice->Start(0);

		return S_OK;
	}

	// ループ再生するものは変更なし
	IXAudio2SourceVoice* pVoice = m_apSourceVoice[label];

	XAUDIO2_VOICE_STATE state;
	pVoice->GetState(&state);

	pVoice->SetVolume(SOUND_INFO[label].Volume);

	if (state.BuffersQueued != 0)
	{
		pVoice->Stop(0);
		pVoice->FlushSourceBuffers();
	}

	pVoice->SubmitSourceBuffer(&buffer);
	pVoice->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::PlaySound(SOUND_LABEL label, std::function<void()> onFinish)
{
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	if (SOUND_INFO[label].nCntLoop == 0)
	{// SEの時にだけコールバックする
		CSoundCallBack* callback = new CSoundCallBack();
		callback->onFinish = onFinish;

		IXAudio2SourceVoice* pVoice = nullptr;

		HRESULT hr = m_pXAudio2->CreateSourceVoice(
			&pVoice,
			&m_aWfx[label],
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			callback
		);

		if (FAILED(hr))
		{
			delete callback;
			return hr;
		}

		pVoice->SubmitSourceBuffer(&buffer);
		pVoice->Start(0);

		return S_OK;
	}

	// BGMなど繰り返し再生するもの
	return PlaySound(label);
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void CSound::Stop(SOUND_LABEL label)
{
	// デュエルBGM専用処理
	if (label == SOUND_LABEL_BGM_DUEL)
	{
		if (m_pDuelBGMVoice)
		{
			m_pDuelBGMVoice->Stop(0);
			m_pDuelBGMVoice->FlushSourceBuffers();
			m_pDuelBGMVoice->DestroyVoice();
			m_pDuelBGMVoice = nullptr;
		}
		return;
	}

	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	// 音量取得
	m_apSourceVoice[label]->SetVolume(SOUND_INFO[label].Volume);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}


}

//=============================================================================
// 音が鳴り終わったか判断(全て)
//=============================================================================
bool CSound::IsPlaySound(SOUND_LABEL label)
{
	if (m_apSourceVoice[label] == nullptr)
	{
		return false;
	}

	XAUDIO2_VOICE_STATE state;
	m_apSourceVoice[label]->GetState(&state);

	return (state.BuffersQueued > 0);
}

//=============================================================================
// 戦闘用BGMリスト読み込み
//=============================================================================
void CSound::LoadDuelBGMList()
{
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("data\\SOUND\\BGM\\duel\\*.wav", &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			m_DuelBGMList.push_back(std::string("data\\SOUND\\BGM\\duel\\") + fd.cFileName);
		} while (FindNextFileA(hFind, &fd));

		FindClose(hFind);
	}
}

//=============================================================================
// 音声再生処理
//=============================================================================
HRESULT CSound::PlaySoundFromFile(const char* filename, float volume, int loopCount)
{
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;

	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// RIFFチャンク
	HRESULT hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	if (dwFiletype != 'EVAW')
	{
		return S_FALSE;
	}

	// fmtチャンク
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	// dataチャンク
	DWORD audioSize = 0;
	hr = CheckChunk(hFile, 'atad', &audioSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	BYTE* audioData = (BYTE*)malloc(audioSize);
	hr = ReadChunkData(hFile, audioData, audioSize, dwChunkPosition);
	if (FAILED(hr))
	{
		return hr;
	}

	CloseHandle(hFile);

	// SourceVoice作成
	IXAudio2SourceVoice* pVoice = nullptr;
	hr = m_pXAudio2->CreateSourceVoice(&pVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		return hr;
	}

	buffer.AudioBytes = audioSize;
	buffer.pAudioData = audioData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = loopCount;

	pVoice->SetVolume(volume);
	pVoice->SubmitSourceBuffer(&buffer);
	pVoice->Start(0);

	m_pDuelBGMVoice = pVoice;	// デュエル専用のため保存
	return S_OK;
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}