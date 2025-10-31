//***************************************************************************************************
//
// imgui(manager.cpp)
// Author : Kasai Keisuke
//
//***************************************************************************************************

// include
#include "input.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 */
My::CManager::CManager():
	m_pRenderer(nullptr),		// レンダラーのポインタ
	m_pInputKeyboard(nullptr),	// キーボードのポインタ
	m_pInputMouse(nullptr)		// マウスのポインタ
{

}

/**
 * @brief デストラクタ
 */
My::CManager::~CManager()
{

}

/**
* @brief 初期化処理
* @param [in]インスタンス
* @param [in]ウィンドウハンドル
* @param [in]フラグ
* @return 成功したかどうか
*/
HRESULT My::CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	m_pRenderer = new CRenderer();

	// キーボードの生成
	m_pInputKeyboard = new CInputKeyboard();

	// マウスの生成
	m_pInputMouse = new CInputMouse();

	// レンダラーの初期化
	m_pRenderer->Init(hWnd, bWindow);

	// キーボードの初期化
	m_pInputKeyboard->Init(hInstance, hWnd);

	// マウスの初期化
	m_pInputMouse->Init(hInstance, hWnd);

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CManager::Uninit()
{
	// キーボードの終了処理
	m_pInputKeyboard->Uninit();

	// マウスの終了処理
	m_pInputMouse->Uninit();

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();

		delete m_pRenderer;

		m_pRenderer = nullptr;
	}

}

/**
* @brief 更新処理
*/
void My::CManager::Update()
{
	// レンダラーの更新
	m_pRenderer->Update();

	// キーボードの更新処理
	m_pInputKeyboard->Update();

	// マウスの更新処理
	m_pInputMouse->Update();

}

/**
* @brief 描画処理
*/
void My::CManager::Draw()
{
	// レンダラーの描画
	m_pRenderer->Draw();
}

/**
* @brief レンダラー取得処理
* @return レンダラーのポインタ
*/
My::CRenderer* My::CManager::GetRenderer()
{
	return m_pRenderer;
}

/**
* @brief キーボード取得処理
* @return キーボードのポインタ
*/
My::CInputKeyboard* My::CManager::GetKeyboard()
{
	return m_pInputKeyboard;
}

/**
* @brief マウス取得処理
* @return マウスのポインタ
*/
My::CInputMouse* My::CManager::GetMouse()
{
	return m_pInputMouse;
}

