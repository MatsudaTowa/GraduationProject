//***************************************************************************************************
//
// imgui(input.cpp)
// Author : Kasai Keisuke
//
//***************************************************************************************************

// include
#include "input.h"

// 静的メンバ変数初期化
LPDIRECTINPUT8 My::CInput::m_pInput = nullptr;

/**
 * @brief コンストラクタ
 */
My::CInput::CInput():
	m_pDevice(nullptr),
	m_nPressTime(0)
{

}

/**
 * @brief デストラクタ
 */
My::CInput::~CInput()
{
	if (m_pInput != nullptr)
	{
		m_pInput->Release();

		m_pInput = nullptr;
	}

}

/**
 * @brief 初期化処理
 * @param [in]インスタンス
 * @param [in]ウィンドウハンドル
 */
HRESULT My::CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		// DirectInputデバイスオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}

	}

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CInput::Uninit()
{
	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// ポインタの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}

}

//***********************************
// これより下はキーボード
//***********************************

/**
* @brief コンストラクタ
*/
My::CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;			// プレス情報
		m_aKeyStateTrigger[nCntKey] = NULL;		// トリガー情報
	}

}

/**
* @brief デストラクタ
*/
My::CInputKeyboard::~CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;			// プレス情報
		m_aKeyStateTrigger[nCntKey] = NULL;		// トリガー情報
	}

}

/**
* @brief 初期化処理
* @param [in]インスタンス
* @param [in]ウィンドウハンドル
* @return 成功したかどうか
*/
HRESULT My::CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// デバイスの初期化
	CInput::Init(hInstance,hWnd);

	// 入力デバイス(キーボードの生成)
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマット
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

/**
* @brief 更新処理
*/
void My::CInputKeyboard::Update()
{
	// ローカル変数宣言
	BYTE aKeyState[NUM_KEY];	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	// キーボードのトリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードのアクセス権を獲得
	}

}

/**
* @brief プレス取得処理
* @param [in]キー情報
* @return 押したかどうか
*/
bool My::CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

/**
* @brief トリガー取得処理
* @param [in]キー情報
* @return 押したかどうか
*/
bool My::CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//***********************************
// これより下はマウス
//***********************************

/**
* @brief コンストラクタ
*/
My::CInputMouse::CInputMouse()
{
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	ZeroMemory(&m_MouseStateTrigger, sizeof(m_MouseStateTrigger));
}

/**
* @brief デストラクタ
*/
My::CInputMouse::~CInputMouse()
{
}

/**
* @brief 初期化処理
*/
HRESULT My::CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (!m_pInput)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	// デバイス生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// アクセス権取得
	m_pDevice->Acquire();

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CInputMouse::Uninit()
{
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

/**
* @brief 更新処理
*/
void My::CInputMouse::Update()
{
	DIMOUSESTATE prevState = m_MouseState;

	if (FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
	{
		m_pDevice->Acquire();
		return;
	}

	for (int i = 0; i < MAX_BUTTON; i++)
	{
		m_MouseStateTrigger.rgbButtons[i] = (!prevState.rgbButtons[i] && m_MouseState.rgbButtons[i]) ? 0x80 : 0x00;
	}
}

/**
* @brief プレス取得処理
*/
bool My::CInputMouse::GetPress(int button)
{
	// マウスのボタン範囲外かどうかチェック
	if (button < 0 || button >= MAX_BUTTON)
	{
		return false;
	}
	else
	{
		return(m_MouseState.rgbButtons[button] & 0x80) != 0;
	}
}

/**
* @brief トリガー取得処理
*/
bool My::CInputMouse::GetTrigger(int button)
{
	// マウスのボタン範囲外かどうかチェック
	if (button < 0 || button >= MAX_BUTTON)
	{
		return false;
	}
	else
	{
		return (m_MouseStateTrigger.rgbButtons[button] & 0x80) != 0;
	}
}

/**
* @brief マウスの移動量取得(X軸)
*/
LONG My::CInputMouse::GetDeltaX()
{
	return m_MouseState.lX;
}

/**
* @brief マウスの移動量取得(Y軸)
*/
LONG My::CInputMouse::GetDeltaY()
{
	return m_MouseState.lY;
}

/**
* @brief マウスの移動量取得(マウスホイール)
*/
LONG My::CInputMouse::GetDeltaZ()
{
	return m_MouseState.lZ;
}
