//=============================================
//
//3DTemplate[manager.cpp]
//Author Matsuda Towa
//
//=============================================
#include "manager.h"
#include "renderer.h"

/**
 * @brief コンストラクタ
 */
My::CManager::CManager():
m_pFade(nullptr),		//フェードのポインタ
m_pCamera(),		//カメラのポインタ	
m_pColision(nullptr),	//コリジョンのポインタ
m_pKeyboard(nullptr),	//キーボードのポインタ
m_pLight(nullptr),		//ライトのポインタ
m_pModel(nullptr),		//モデルポインタ
m_pMouse(nullptr),		//マウスのポインタ
m_pPad(nullptr),		//padのポインタ
m_pRenderer(nullptr),	//レンダラーポインタ
m_pScene(nullptr),		//シーンポインタ
m_pTexture(nullptr),		//テクスチャ管理ポインタ
m_CurrentViewCamera(INT_ZERO)	//映すカメラ番号
{
	
}

/**
 * @brief デストラクタ
 */
My::CManager::~CManager()
{
}

/**
 * @brief 初期化
 * @param [in]ハンドルインスタンス
 * @param [in]ハンドルワンド
 * @param [in]ウィンドウモード
 */
HRESULT My::CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

	m_hWnd = hWnd;

	//生成＆初期化
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer();
		m_pRenderer->Init(hWnd,bWindow);
	}
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboard();

		//キーボードの初期化処理
		if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse();

		//マウスの初期化処理
		if (FAILED(m_pMouse->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}
	if (m_pPad == nullptr)
	{
		m_pPad = new CInputPad();

		//マウスの初期化処理
		if (FAILED(m_pPad->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	//シーン生成
	if (m_pScene == nullptr)
	{
		m_pScene = new CScene();
		m_pScene->Init();

	}

	//ライト生成
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight();
		m_pLight->Init();
	}

	//テクスチャ生成
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture();
	}

	//モデル生成
	if (m_pModel == nullptr)
	{
		m_pModel = new CModel();
	}

	//当たり判定生成
	if (m_pColision == nullptr)
	{
		m_pColision = new CColision();
	}

	//最初のシーン設定
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(VEC3_RESET_ZERO);
	}

	SetMode(CScene::MODE::MODE_TITLE);

	return S_OK;
}

/**
 * @brief 終了
 */
void My::CManager::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pModel != nullptr)
	{
		m_pModel->Unload();
		delete m_pModel;
		m_pModel = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (m_pColision != nullptr)
	{
		delete m_pColision;
		m_pColision = nullptr;
	}

	ClearCamera();

	//キーボードの終了処理
	m_pKeyboard->Uninit();
	//マウスの終了処理
	m_pMouse->Uninit();
	//Padの終了処理
	m_pPad->Uninit();

	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	CObject::ReleaseAll();
}

/**
 * @brief 更新
 */
void My::CManager::Update()
{
	//更新処理
	m_pRenderer->Update();

	for (auto& itr : m_pCamera)
	{
		if (itr == nullptr)
		{
			continue;
		}
		itr->Update();
	}

	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pPad->Update();

	//シーンの更新
	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	CObject::UpdateAll();
}

/**
 * @brief 描画
 */
void My::CManager::Draw()
{
	//描画処理
	m_pRenderer->Draw();

	//シーンの描画
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

/**
 * @brief シーン設定
 * @param [in]次のシーン
 */
void My::CManager::SetMode(CScene::MODE mode)
{
	//シーン終了
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//シーン切り替え
	if (m_pScene == nullptr)
	{
		m_pScene = CScene::Create(mode);
	}
}


/**
 * @brief レンダラー取得
 */
My::CRenderer* My::CManager::GetRenderer()
{
	return m_pRenderer;
}

/**
 * @brief キーボード取得
 */
My::CInputKeyboard* My::CManager::GetKeyboard()
{
	return m_pKeyboard;
}

/**
 * @brief pad取得
 */
My::CInputMouse* My::CManager::GetMouse()
{
	return m_pMouse;
}

/**
 * @brief マウス取得
 */
My::CInputPad* My::CManager::GetPad()
{
	return m_pPad;
}

/**
 * @brief カメラ取得
 */
My::CCamera* My::CManager::GetCamera(int idx)
{
	if (idx >= m_pCamera.size())
	{
		return nullptr;
	}
	return m_pCamera[idx];
}

/**
 * @brief カメラの個数
 */
int My::CManager::GetCameraSize()
{
	return m_pCamera.size();
}

/**
 * @brief 映すカメラの番号取得
 */
int My::CManager::GetCureentViewCamera()
{
	return m_CurrentViewCamera;
}

/**
 * @brief 映すカメラの番号設定
 */
void My::CManager::SetCureentViewCamera(int idx)
{
	if (idx >= m_pCamera.size())
	{
		assert(false && "カメラのサイズオーバー");
	}

	m_CurrentViewCamera = idx;
}

/**
 * @brief カメラ全削除
 */
void My::CManager::ClearCamera()
{
	for (auto& itr : m_pCamera)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		delete itr;
		itr = nullptr;
	}
	m_pCamera.clear();
}

/**
 * @brief 指定されたカメラ削除
 */
void My::CManager::EraseCamera(unsigned int idx)
{
	if (idx >= m_pCamera.size())
	{
		return;
	}
	CCamera* camera = m_pCamera[idx];
	camera->Uninit();
	delete camera;
	camera = nullptr;

	m_pCamera.erase(m_pCamera.begin() + idx);
}

/**
 * @brief ライト取得
 */
My::CLight* My::CManager::GetLight()
{
	return m_pLight;
}

/**
 * @brief テクスチャ取得
 */
My::CTexture* My::CManager::GetTexture()
{
	return m_pTexture;
}

/**
 * @brief モデル取得
 */
My::CModel* My::CManager::GetModel()
{
	return m_pModel;
}

/**
 * @brief フェード取得
 */
My::CFade* My::CManager::GetFade()
{
	return m_pFade;
}

/**
 * @brief コリジョン取得
 */
My::CColision* My::CManager::GetColision()
{
	return m_pColision;
}
