//=============================================
//
//インプット[input.cpp]
//Author Matsuda Towa
//
//=============================================
#include "input.h"
#include "manager.h"

//静的メンバ初期化
LPDIRECTINPUT8 My::CInput::m_pInput = nullptr;

/**
 * @brief コンストラクタ
 */
My::CInput::CInput():
m_pDevice(nullptr)	//デバイス初期化
{
}

/**
 * @brief デストラクタ
 */
My::CInput::~CInput()
{
}

/**
 * @brief 初期化
 * @param [in]ハンドルインスタンス
 * @param [in]ハンドルワンド
 * @return 成功したかどうか
 */
HRESULT My::CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CInput::Uninit()
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//↓からキーボード

/**
 * @brief コンストラクタ
 */
My::CInputKeyboard::CInputKeyboard()
{
}

/**
 * @brief デストラクタ
 */
My::CInputKeyboard::~CInputKeyboard()
{
}


/**
 * @brief 初期化
 * @param [in]ハンドルインスタンス
 * @param [in]ハンドルワンド
 * @return 成功したかどうか
 */
HRESULT My::CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	My::CInput::Init(hInstance,hWnd);
	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードのアクセス権を獲得
	m_pDevice->Acquire();
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CInputKeyboard::Uninit()
{
	My::CInput::Uninit();
}

/**
 * @brief 更新
 */
void My::CInputKeyboard::Update()
{
	BYTE aKeyState[NUM_KEY_MAX]; //入力情報
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = INT_ZERO; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey]; //リリース
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire(); //キーボードのアクセス権を獲得
	}
}
//↓からマウス

/**
 * @brief コンストラクタ
 */
My::CInputMouse::CInputMouse()
{
#ifdef _DEBUG
	m_faster_move = VEC3_RESET_ZERO;
#endif // _DEBUG

}

/**
 * @brief デストラクタ
 */
My::CInputMouse::~CInputMouse()
{
}

/**
 * @brief 初期化
 * @param [in]ハンドルインスタンス
 * @param [in]ハンドルワンド
 * @return 成功したかどうか
 */
HRESULT My::CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// デバイスの設定
	DIPROPDWORD diprop;
	m_MousePos = VEC3_RESET_ZERO;
	m_MouseMove = VEC3_RESET_ZERO;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = INT_ZERO;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;


	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	//マウスのアクセス権を獲得
	m_pDevice->Acquire();
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CInputMouse::Uninit()
{
	CInput::Uninit();
}

/**
 * @brief 更新
 */
void My::CInputMouse::Update()
{

	DIMOUSESTATE zdiMouseState;
	int nCntMouse;
	
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(zdiMouseState), &zdiMouseState)))
	{
		for (nCntMouse = INT_ZERO; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			m_KeyStateRelease.rgbButtons[nCntMouse] = (m_KeyState.rgbButtons[nCntMouse] ^ zdiMouseState.rgbButtons[nCntMouse]) & ~zdiMouseState.rgbButtons[nCntMouse];
			m_KeyStateTrigger.rgbButtons[nCntMouse] = (m_KeyState.rgbButtons[nCntMouse] ^ zdiMouseState.rgbButtons[nCntMouse]) & zdiMouseState.rgbButtons[nCntMouse];
			m_KeyState.rgbButtons[nCntMouse] = zdiMouseState.rgbButtons[nCntMouse]; //キーボードのプレス情報を保存
		}
		
		m_MouseMove.x = (float)zdiMouseState.lX;
		m_MouseMove.y = (float)zdiMouseState.lY;

#ifdef _DEBUG
		float x = m_MouseMove.x;
		float y = m_MouseMove.y;

		std::abs(x);
		std::abs(y);
		if (m_faster_move.x < x)
		{
			m_faster_move.x = x;
		}
		if (m_faster_move.y < y)
		{
			m_faster_move.y = y;
		}
#endif // _DEBUG



		UpdateAngle();
	}
	else
	{
		m_pDevice->Acquire(); //キーボードのアクセス権を獲得
	}

}

/**
 * @brief 方向更新
 */
void My::CInputMouse::UpdateAngle()
{
	float x = m_MouseMove.x;
	float y = m_MouseMove.y;

	// 方向を調べる
	if (y > 0)
	{
		//方向入力フラグを初期化
		for (int i = 0; i < MAX; ++i)
		{
			m_isAngle[i] = false;
		}

		if (x > 0)
		{ // 右上
			m_isAngle[RIGHT] = true;
			m_isAngle[DOWN] = true;
		}
		else if (x < 0)
		{ // 左上
			m_isAngle[LEFT] = true;
			m_isAngle[DOWN] = true;
		}
		else
		{ // 上
			m_isAngle[DOWN] = true;
		}
	}
	else if (y < 0)
	{
		//方向入力フラグを初期化
		for (int i = 0; i < MAX; ++i)
		{
			m_isAngle[i] = false;
		}
		if (x > 0)
		{ // 右下
			m_isAngle[RIGHT] = true;
			m_isAngle[UP] = true;
		}
		else if (x < 0)
		{ // 左下
			m_isAngle[LEFT] = true;
			m_isAngle[UP] = true;
		}
		else
		{ // 下
			m_isAngle[UP] = true;

		}
	}
	else
	{
		if (x > 0)
		{ // 右
			m_isAngle[RIGHT] = true;
		}
		else if (x < 0)
		{ // 左
			m_isAngle[LEFT] = true;
		}
	}
}

/**
 * @brief マウスのデバッグ表示
 */
void My::CInputMouse::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	POINT pMouseMove;
	GetCursorPos(&pMouseMove);
	sprintf(&aStr[0], "[mouse]\nmove:%.1f,%.1f,%.1f\n最速値:%.1f,%.1f\n\npos:%d,%d"
		, m_MouseMove.x, m_MouseMove.y, m_MouseMove.z,m_faster_move.x,m_faster_move.y ,pMouseMove.x, pMouseMove.y);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));

	DebugAngle();
#endif // _DEBUG
}

/**
 * @brief マウスのデバッグ表示
 */
void My::CInputMouse::DebugAngle()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n上:%d 右:%d 左:%d 下:%d"
		,m_isAngle[UP],m_isAngle[RIGHT],m_isAngle[LEFT],m_isAngle[DOWN]);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}


//↓からpad

/**
 * @brief コンストラクタ
 */
My::CInputPad::CInputPad():m_Connect(false)
{
}

/**
 * @brief デストラクタ
 */
My::CInputPad::~CInputPad()
{
}

/**
 * @brief 初期化
 * @param [in]ハンドルインスタンス
 * @param [in]ハンドルワンド
 * @return 成功したかどうか
 */
HRESULT My::CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_Connect = false;

	XINPUT_STATE joykeyState; //ジョイパッドの入力情報

	//メモリのクリア
	memset(&m_joyKeyState, INT_ZERO, sizeof(XINPUT_STATE));

	//メモリのクリア
	memset(&m_JoypadMotor, INT_ZERO, sizeof(XINPUT_VIBRATION));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	//ジョイパッドの状態を取得
	if (XInputGetState(INT_ZERO, &joykeyState) == ERROR_SUCCESS)
	{
		m_Connect = true;
	}
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CInputPad::Uninit()
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

/**
 * @brief 更新
 */
void My::CInputPad::Update()
{
	XINPUT_STATE joykeyState; //ジョイパッドの入力情報

		//ジョイパッドの状態を取得
	if (XInputGetState(INT_ZERO, &joykeyState) == ERROR_SUCCESS)
	{
		//接続されているかどうか
		m_Connect = true;

		//スティックの更新
		UpdateStick(joykeyState);

		joykeyState.Gamepad.wButtons |= GetJoypadStick(joykeyState.Gamepad.sThumbLX, joykeyState.Gamepad.sThumbLY, JOYPAD_ZONE);

		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joyKeyState;
		m_joyKeyStateTrigger = Button & ~OldButton;    // トリガー処理
		m_ajoyKeyStateRelease = ~Button & OldButton;    // リリース処理

		// 現在の時間を取得する
		m_aJoypadCurrentTime = timeGetTime();

		if (joykeyState.Gamepad.wButtons && ((m_aJoypadCurrentTime - m_aJoypadExecLastTime) > JOYPAD_SPEED))
		{
			// 最後に真を返した時間を保存
			m_aJoypadExecLastTime = m_aJoypadCurrentTime;

			// キーボードのリピート情報を保存
			m_joyKeyStateRepeat = joykeyState.Gamepad.wButtons;
		}
		else
		{
			// キーボードのリピート情報を保存
			m_joyKeyStateRepeat = INT_ZERO;
		}

		m_joyKeyState = joykeyState.Gamepad.wButtons;                // プレス処理
	}
	else
	{
		m_Connect = false;
	}
}

/**
 * @brief スティックの更新
 * @param [in]Xインプットの状態
 */
void My::CInputPad::UpdateStick(XINPUT_STATE state)
{
	for (int nCntStick = 0; nCntStick < STICKTYPE_MAX; nCntStick++)
	{
		float fX,fY = 0.0f; //スティックのX軸、Y軸

		switch (nCntStick)
		{
		case STICKTYPE_LEFT:
			fX = (state.Gamepad.sThumbLX);
			fY = (state.Gamepad.sThumbLY);
			break;
		case STICKTYPE_RIGHT:
			fX = (state.Gamepad.sThumbRX);
			fY = (state.Gamepad.sThumbRY);
			break;
		}

		//角度を取得
		m_stick.afAngle[nCntStick] = FindAngle(D3DXVECTOR3(fX, fY, 0.0f),VEC3_RESET_ZERO) * -1;

		//スティックの倒し具合を取得
		m_stick.afTplDiameter[nCntStick] = fabsf(fX);

		if (m_stick.afTplDiameter[nCntStick] < fabsf(fY))
		{
			m_stick.afTplDiameter[nCntStick] = fabsf(fY);
		}

		m_stick.afTplDiameter[nCntStick] /= 32768.0f; //倒している状態の初期値

		//方向入力フラグを初期化
		for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
		{
			m_bAngle[nCntStick][nCntAngle] = false;
		}

		if (m_stick.afTplDiameter[nCntStick] > 0.1f)
		{
			//角度が四分割で上に位置する時、上フラグを真にする
			if ((m_stick.afAngle[nCntStick] < D3DX_PI * -0.75) || (m_stick.afAngle[nCntStick] > D3DX_PI * 0.75))
			{
				m_bAngle[nCntStick][STICKANGLE_UP] = true;
			}

			//角度が四分割で下に位置する時、下フラグを真にする
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.25))
			{
				m_bAngle[nCntStick][STICKANGLE_DOWN] = true;
			}

			//角度が四分割で左に位置する時、左フラグを真にする
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.75) && (m_stick.afAngle[nCntStick] < D3DX_PI * -0.25))
			{
				m_bAngle[nCntStick][STICKANGLE_LEFT] = true;
			}

			//角度が四分割で右に位置する時、右フラグを真にする
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * 0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.75))
			{
				m_bAngle[nCntStick][STICKANGLE_RIGHT] = true;
			}
		}

		//角度に応じた入力処理
		for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
		{
			//スティックのトリガー情報を保存
			m_stick.abAngleTrigger[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & m_bAngle[nCntStick][nCntAngle];

			//スティックのリリース情報を保存
			m_stick.abAngleRelease[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & ~m_bAngle[nCntStick][nCntAngle];

			//現在の時間を取得
			m_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

			if (m_bAngle[nCntStick][nCntAngle] && ((m_aStickCurrentTime[nCntStick][nCntAngle] - m_aStickExecLastTime[nCntStick][nCntAngle]) > JOYPAD_SPEED))
			{
				//最後に真を返した時間を保存
				m_aStickExecLastTime[nCntStick][nCntAngle] = m_aStickCurrentTime[nCntStick][nCntAngle];

				//スティックのリピート情報を保存
				m_stick.abAngleRepeat[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
			}
			else
			{
				//スティックのリピート情報を保存
				m_stick.abAngleRepeat[nCntStick][nCntAngle] = 0;
			}

			//スティックのプレス情報を保存
			m_stick.abAnglePress[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
		}
	}
}

/**
 * @brief スティックの取得
 */
My::CInputPad::STICKINPUT My::CInputPad::GetStick(void)
{
	return m_stick;
}

/**
 * @brief 角度計算
 * @param [in]位置
 * @param [in]目的の位置
 */
float My::CInputPad::FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos)
{
	float fAngle; //角度

	fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1.0f;

	return fAngle;
}

/**
 * @brief スティックの取得
 * @param [in]Xの倒されている値
 * @param [in]Yの倒されている値
 * @param [in]値制限
 */
WORD My::CInputPad::GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}
