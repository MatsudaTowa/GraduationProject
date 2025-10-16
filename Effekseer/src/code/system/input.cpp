//============================================
//
// 入力[input.cpp]
// Author:Uedakou
// 
//============================================
#include "input.h"	// 入力処理

namespace My {
	// 静的メンバ
	LPDIRECTINPUT8 CInput::m_pInput = nullptr;

	/// <summary>
	/// コンストラクタ（基底）
	/// </summary>
	CInput::CInput() : m_pDevice(nullptr)
	{
	}
	/// <summary>
	/// デストラクタ（基底）
	/// </summary>
	CInput::~CInput()
	{
	}
	/// <summary>
	/// 初期化（基底）
	/// </summary>
	/// <param name="hInstance">アプリケーションインスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <returns>S_OK：成功、E_FAIL：失敗</returns>
	HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
	{
		if (m_pInput == nullptr)
		{
			if (FAILED(DirectInput8Create(hInstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void**)&m_pInput, nullptr)))
			{
				return E_FAIL;
			}
		}
		return S_OK;
	}
	/// <summary>
	/// 終了処理（基底）
	/// </summary>
	void CInput::Uninit()
	{
		// 入力デバイスの破棄
		if (m_pDevice != nullptr)
		{
			m_pDevice->Unacquire();	// デバイスへのアクセス権を放棄
			m_pDevice->Release();
			m_pDevice = nullptr;
		}
		// DirectInputオブジェクトの放棄
		if (m_pInput != nullptr)
		{
			m_pInput->Release();
			m_pInput = nullptr;
		}
	}
	/// <summary>
	/// 更新（基底）
	/// </summary>
	void CInput::Update()
	{

	}
	//============================================
	// コンスト（キーボード）
	//============================================
	CInputKeyboard::CInputKeyboard()
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			m_aPless[nCnt] = 0;
			m_aTrigger[nCnt] = 0;
			m_aRepeat[nCnt] = 0;
			m_aRelease[nCnt] = 0;
		}
	}
	//============================================
	// デストラ（キーボード）
	//============================================
	CInputKeyboard::~CInputKeyboard()
	{
	}
	//============================================
	// 初期化（キーボード）
	//============================================
	HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
	{
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
		// 入力デバイス(キーボード)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard,
			&m_pDevice, nullptr)))
		{
			return E_FAIL;
		}
		// データフォーマットを設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}
		// 協調モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			return E_FAIL;
		}
		// キーボードへのアクセス権を獲得
		m_pDevice->Acquire();
		return S_OK;
	}
	//============================================
	// 終了（キーボード）
	//============================================
	void CInputKeyboard::Uninit()
	{
		CInput::Uninit();
	}
	//============================================
	// 更新（キーボード）
	//============================================
	void CInputKeyboard::Update()
	{
		BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
		int nCntKey;
		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
		{
			for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
			{
				// キーボードのリリース情報を保存
				m_aRelease[nCntKey] = (m_aPless[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
				// キーボードのリピート情報を保存
				m_aRepeat[nCntKey] = (m_aPless[nCntKey] & aKeyState[nCntKey]);
				// キーボードのトリガー情報を保存
				m_aTrigger[nCntKey] = (m_aPless[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
				// キーボードのプレス情報を保存
				m_aPless[nCntKey] = aKeyState[nCntKey];
				// プレスは最後
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
	/// <summary>
	/// 全キープレス取得
	/// </summary>
	/// <returns>キーのプレス情報</returns>
	bool CInputKeyboard::GetPless()	// プレス
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if ((m_aPless[nCnt] & 0x80) != 0)
			{
				return true;
			}
		}
		return false;
	}
	/// <summary>
	/// 全キートリガー取得
	/// </summary>
	/// <returns>キーのトリガー情報</returns>
	bool CInputKeyboard::GetTrigger()
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if ((m_aTrigger[nCnt] & 0x80) != 0)
			{
				return true;
			}
		}
		return false;
	}
	/// <summary>
	/// 全キーリピート取得
	/// </summary>
	/// <returns>キーのリピート情報</returns>
	bool CInputKeyboard::GetRepeat()
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if ((m_aRepeat[nCnt] & 0x80) != 0)
			{
				return true;
			}
		}
		return false;
	}
	/// <summary>
	/// 全キーリリース取得
	/// </summary>
	/// <returns>キーのリリース情報</returns>
	bool CInputKeyboard::GetRelease()
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if ((m_aRelease[nCnt] & 0x80) != 0)
			{
				return true;
			}
		}
		return false;
	}
	//============================================
	// コンスト(マウス)
	//============================================
	CInputMouse::CInputMouse()
	{
		m_lHweel = 0;
	}
	//============================================
	// デストラクタ(マウス)
	//============================================
	CInputMouse::~CInputMouse()
	{
	}
	//============================================
	// 初期化(マウス)
	//============================================
	HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
	{
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
		// 入力デバイス(マウス)を作成
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
		{
			// デバイスの作成に失敗
			return E_FAIL;
		}
		// データフォーマットを設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
		{
			// データフォーマットに失敗
			return E_FAIL;
		}
		// モードを設定（フォアグラウンド＆非排他モード）
		if (FAILED(m_pDevice->SetCooperativeLevel(FindWindowA(CLASS_NAME, nullptr), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			// モードの設定に失敗
			return E_FAIL;
		}

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

		if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			// デバイスの設定に失敗
			return E_FAIL;
		}

		// 入力制御開始
		m_pDevice->Acquire();

		return S_OK;
	}
	//============================================
	// 終了(マウス)
	//============================================
	void CInputMouse::Uninit()
	{
		CInput::Uninit();
	}
	//============================================
	// 更新(マウス)
	//============================================
	void CInputMouse::Update()
	{
		D3DXVECTOR2 PosOld = m_MousePos;	// 1フレーム前位置保存用
		POINT p;	// 現在位置保存用
		GetCursorPos(&p);
		ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &p);

		m_MousePos.x = (float)p.x;
		m_MousePos.y = (float)p.y;


		m_MouseMove.x = m_MousePos.x - PosOld.x;
		m_MouseMove.y = m_MousePos.y - PosOld.y;

		m_PrevMouseState = m_CurrentMouseState;
		// マウスの状態を取得します
		HRESULT	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
		if (FAILED(hr))
		{
			m_pDevice->Acquire();
			hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
			m_lHweel = m_CurrentMouseState.lZ;
		}

	}
	//============================================
	// プレス取得(マウス)
	//============================================
	bool CInputMouse::GetPress(MouseButton button_type)
	{
		if (m_CurrentMouseState.rgbButtons[button_type] != 0)
		{
			return true;
		}
		return false;
	}
	//============================================
	// トリガー取得(マウス)
	//============================================
	bool CInputMouse::GetTrigger(MouseButton button_type)
	{
		if (!(m_PrevMouseState.rgbButtons[button_type] & ON_VALUE) &&
			m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE)
		{
			return true;
		}
		return false;
	}
	//============================================
	// リピート取得(マウス)
	//============================================
	bool CInputMouse::GetRepeat(MouseButton button_type)
	{
		if (m_PrevMouseState.rgbButtons[button_type] & ON_VALUE &&
			m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE)
		{
			return true;
		}

		return false;
	}
	//============================================
	// リリース取得(マウス)
	//============================================
	bool CInputMouse::GetRelease(MouseButton button_type)
	{
		if (m_PrevMouseState.rgbButtons[button_type] & ON_VALUE &&
			!(m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE))
		{
			return true;
		}

		return false;
	}
	//============================================
	// 位置取得(マウス)
	//============================================
	D3DXVECTOR2 CInputMouse::GetPos()
	{
		return m_MousePos;
	}
	//============================================
	// 動き取得(マウス)
	//============================================
	D3DXVECTOR2 CInputMouse::GetMove()
	{
		return m_MouseMove;
	}
	//============================================
	// ホイール取得(マウス)
	//============================================
	int CInputMouse::GetMouseHweel()
	{
		return m_lHweel;
	}
	//============================================
	// コンスト
	//============================================
	CInputJoypad::CInputJoypad()
	{
		memset(&m_joiKeyState, 0, sizeof(::XINPUT_STATE));
		memset(&m_joiKeyStateTrigger, 0, sizeof(::XINPUT_STATE));
		memset(&m_joiKeyStateRepeat, 0, sizeof(::XINPUT_STATE));
		memset(&m_joiKeyStateRelease, 0, sizeof(::XINPUT_STATE));
	}
	//============================================
	// デストラ
	//============================================
	CInputJoypad::~CInputJoypad()
	{
	}
	//============================================
	// 初期化
	//============================================
	HRESULT CInputJoypad::Init()
	{
		// XInputのステートを設定(有効にする)
		XInputEnable(true);
		return S_OK;
	}
	//============================================
	// 終了
	//============================================
	void CInputJoypad::Uninit()
	{
		// XInputのステートを設定(無効にする)
		XInputEnable(false);
	}
	//============================================
	// 更新
	//============================================
	void CInputJoypad::Update()
	{
		::XINPUT_STATE joykeyState;	// ジョイパッドの情報
		//スティックのX軸、Y軸
		float fX = 0.0f;
		float fY = 0.0f;
		// ジョイパッドの状態を取得
		if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
		{
			// リリース
			m_joiKeyStateRelease.Gamepad.wButtons = (m_joiKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & ~joykeyState.Gamepad.wButtons;
			// リピート
			m_joiKeyStateRepeat.Gamepad.wButtons = (m_joiKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);
			// トリガー
			m_joiKeyStateTrigger.Gamepad.wButtons = (m_joiKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);
			// プレス
			m_joiKeyState = joykeyState;	// ジョイパッドのプレス情報を保存

			// コントローラースティック
			for (int nCntStick = 0; nCntStick < JOYSTICK_MAX; nCntStick++)
			{
				switch (nCntStick)
				{
				case JOYSTICK_LEFT:
					fX = (float)(joykeyState.Gamepad.sThumbLX);
					fY = (float)(joykeyState.Gamepad.sThumbLY);
					break;
				case JOYSTICK_RIGHT:
					fX = (float)(joykeyState.Gamepad.sThumbRX);
					fY = (float)(joykeyState.Gamepad.sThumbRY);
					break;
				}

				float fAngle; //角度

				D3DXVECTOR3 pos = D3DXVECTOR3(fX, fY, 0.0f);
				D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) * -1;
				fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

				fAngle -= (D3DX_PI * 0.5f);
				fAngle *= -1.0f;

				//角度を取得
				m_stickState[nCntStick].afRot = fAngle;

				//スティックの倒し具合を取得
				m_stickState[nCntStick].afSlope = fabsf(fX);

				if (m_stickState[nCntStick].afSlope < fabsf(fY))
				{
					m_stickState[nCntStick].afSlope = fabsf(fY);
				}

				m_stickState[nCntStick].afSlope /= 32768.0f; //倒している状態の初期値

				bool g_bAngle[STICKANGLE_MAX];
				//方向入力フラグを初期化
				for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
				{
					g_bAngle[nCntAngle] = false;

					if (m_stickState[nCntStick].afSlope > 0.1f)
					{
						//角度調節
						if (m_stickState[nCntStick].afRot < -D3DX_PI)
						{// ?
							m_stickState[nCntStick].afRot = D3DX_PI - (D3DX_PI + m_stickState[nCntStick].afRot) * -1.0f;
						}

						//角度が四分割で上に位置する時、上フラグを真にする
						if ((m_stickState[nCntStick].afRot < D3DX_PI * -0.625f) || (m_stickState[nCntStick].afRot > D3DX_PI * 0.625f))
						{
							g_bAngle[STICKANGLE_UP] = true;
						}
						//角度が四分割で下に位置する時、下フラグを真にする
						else if ((m_stickState[nCntStick].afRot > D3DX_PI * -0.375f) && (m_stickState[nCntStick].afRot < D3DX_PI * 0.375f))
						{
							g_bAngle[STICKANGLE_DOWN] = true;
						}
						//角度が四分割で右に位置する時、右フラグを真にする
						else if ((m_stickState[nCntStick].afRot > D3DX_PI * 0.125f) && (m_stickState[nCntStick].afRot < D3DX_PI * 0.875f))
						{
							g_bAngle[STICKANGLE_RIGHT] = true;
						}
						//角度が四分割で左に位置する時、左フラグを真にする
						else if ((m_stickState[nCntStick].afRot > D3DX_PI * -0.125f) && (m_stickState[nCntStick].afRot < D3DX_PI * -0.875f))
						{
							g_bAngle[STICKANGLE_LEFT] = true;
						}
					}

					// スティックのトリガー情報を保存
					m_stickState[nCntStick].abAngleTrigger[nCntAngle] = (m_stickState[nCntStick].abAnglePress[nCntAngle] ^ g_bAngle[nCntAngle]) & g_bAngle[nCntAngle];
					// スティックのリピート情報を保存
					m_stickState[nCntStick].abAngleRepeat[nCntAngle] =
						// スティックのリリース情報を保存
						m_stickState[nCntStick].abAngleRelease[nCntAngle] = (m_stickState[nCntStick].abAnglePress[nCntAngle] & g_bAngle[nCntAngle]) & g_bAngle[nCntAngle];
					//スティックのプレス情報を保存
					m_stickState[nCntStick].abAnglePress[nCntAngle] = g_bAngle[nCntAngle];
				}
			}
		}
	}
	//============================================
	// ジョイキー入力取得
	//============================================
	// プレス
	bool CInputJoypad::GetJoykyePress(JOYKEY kye)
	{
		return (m_joiKeyState.Gamepad.wButtons & (0x01 << kye)) != 0;
	}
	// トリガー
	bool CInputJoypad::GetJoykyeTrigger(JOYKEY kye)
	{
		return (m_joiKeyStateTrigger.Gamepad.wButtons & (0x01 << kye)) ? true : false;
	}
	// リピート
	bool CInputJoypad::GetJoykyeRepeat(JOYKEY kye)
	{
		return (m_joiKeyStateRepeat.Gamepad.wButtons & (0x01 << kye)) ? true : false;
	}
	// リリース
	bool CInputJoypad::GetJoykyeRelease(JOYKEY kye)
	{
		return (m_joiKeyStateRelease.Gamepad.wButtons & (0x01 << kye)) ? true : false;
	}
	//============================================
	// ジョイスティック入力取得
	//============================================
	// 角度
	float CInputJoypad::GetJoystickSlope(JOYSTICK stick)
	{
		return m_stickState[stick].afSlope;
	}
	// 向き
	float CInputJoypad::GetJoystickRot(JOYSTICK stick)
	{
		return m_stickState[stick].afRot;
	}
	// プレス
	bool CInputJoypad::GetJoystickPress(JOYSTICK stick, STICKANGLE angle)
	{
		return m_stickState[stick].abAnglePress[angle];
	}
	// トリガー
	bool CInputJoypad::GetJoystickTrigger(JOYSTICK stick, STICKANGLE angle)
	{
		return m_stickState[stick].abAngleTrigger[angle];
	}
	// リピート
	bool CInputJoypad::GetJoystickRepeat(JOYSTICK stick, STICKANGLE angle)
	{
		return m_stickState[stick].abAngleRepeat[angle];
	}
	// リリース
	bool CInputJoypad::GetJoystickRelease(JOYSTICK stick, STICKANGLE angle)
	{
		return m_stickState[stick].abAngleRelease[angle];
	}
}