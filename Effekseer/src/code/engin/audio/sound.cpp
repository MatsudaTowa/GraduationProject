//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : AKIRA TANAKA
//
//===========================================================================
#include "sound.h"
namespace My
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSound::CSound()
	{
		// サウンドファイル情報を初期化（ファイルパスとループ設定）
		g_aSoundInfo[SOUND_TITLE_000] = { "data\\BGM\\Title000.wav", -1 };			// BGM : タイトル
		g_aSoundInfo[SOUND_STAGE_SELECT_000] = { "data\\BGM\\StageSelect000.wav", -1 };	// BGM : リザルト
		g_aSoundInfo[SOUND_STAGE_00] = { "data\\BGM\\Stage000.wav", -1 };			// BGM : ステージ000
		g_aSoundInfo[SE_CHOICE_000] = { "data\\SE\\Choice000.wav", 0 };			// BGM : ステージ000
		g_aSoundInfo[SE_DECISION_000] = { "data\\SE\\Decision000.wav", 0 };			// BGM : ステージ000
		g_aSoundInfo[SE_FOOTSTEPS_000] = { "data\\SE\\Footsteps000.wav", 0 };		// BGM : ステージ000
		g_aSoundInfo[SE_GET_ITEM_000] = { "data\\SE\\GetItem000.wav", 0 };			// BGM : ステージ000


	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CSound::~CSound()
	{
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hWnd">ウィンドウのハンドル（エラーメッセージ表示用）</param>
	/// <returns>初期化結果（成功: S_OK / 失敗: E_FAIL）</returns>
	HRESULT CSound::InitSound(HWND hWnd)
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
			hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
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
			buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

			// オーディオバッファの登録
			m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

			// ファイルをクローズ
			CloseHandle(hFile);
		}

		return S_OK;
	}
	/// <summary>
	/// 終了処理
	/// </summary>
	void CSound::UninitSound()
	{
		// サウンドバッファおよびXAudio2の全リソースを開放

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
		if (m_pMasteringVoice != NULL)
		{
			// マスターボイスの破棄
			m_pMasteringVoice->DestroyVoice();
			m_pMasteringVoice = NULL;
		}

		if (m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();
	}
	/// <summary>
	/// サウンド再生（再生中なら一時停止）
	/// </summary>
	/// <param name="label">再生したいサウンドのラベル</param>
	/// <returns>再生結果（成功: S_OK / 失敗: E_FAIL）</returns>
	HRESULT CSound::PlaySound(SOUND_LABEL label)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[label];
		buffer.pAudioData = m_apDataAudio[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

		// 状態取得
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_apSourceVoice[label]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[label]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

		// 再生
		m_apSourceVoice[label]->Start(0);

		return S_OK;
	}
	/// <summary>
	/// サウンド停止（指定ラベル）
	/// </summary>
	/// <param name="label">停止したいハンドル</param>
	void CSound::StopSound(SOUND_LABEL label)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// 状態取得
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_apSourceVoice[label]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[label]->FlushSourceBuffers();
		}
	}
	/// <summary>
	/// サウンド停止（全て）
	/// </summary>
	void CSound::StopSound()
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
	/// <summary>
	/// サウンドが再生中かどうかを取得
	/// </summary>
	/// <param name="label">対象のサウンドラベル</param>
	/// <returns>true:再生中 / false:再生していない</returns>
	bool CSound::IsPlaySound(SOUND_LABEL label)
	{
		XAUDIO2_VOICE_STATE xa2state;
		// 状態取得
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			return true;
		}
		return false;
	}
	/// <summary>
	/// WAVファイルのチャンク構造を解析し、指定フォーマットのチャンク位置とサイズを取得
	/// </summary>
	/// <param name="hFile">ファイルハンドル</param>
	/// <param name="format">探したいチャンク（例：'fmt 'や'data'）</param>
	/// <param name="pChunkSize">チャンクサイズ格納用ポインタ</param>
	/// <param name="pChunkDataPosition">チャンクデータ開始位置格納ポインタ</param>
	/// <returns>成功：S_OK、失敗：HRESULT</returns>
	HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
	{
		// ファイル内をチャンク単位で探索し、目的のチャンクが見つかれば
		// そのサイズとデータ位置を返す
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

	//// <summary>
	/// ファイルから特定のチャンク位置にあるデータを読み込む
	/// </summary>
	/// <param name="hFile">ファイルハンドル</param>
	/// <param name="pBuffer">読み込み先のバッファ</param>
	/// <param name="dwBuffersize">バッファサイズ</param>
	/// <param name="dwBufferoffset">チャンクデータのオフセット位置</param>
	/// <returns>成功：S_OK、失敗：HRESULT</returns>
	HRESULT CSound::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
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
}
