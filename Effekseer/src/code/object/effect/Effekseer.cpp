//============================================
//
// エフェクト[Effekseer.cpp]
// Author:Uedakou
// 
//============================================
#include "Effekseer.h"  // エフェクシア
#include "../../system/manager.h"  // マネージャー
#include "EffekseerManager.h"   // エフェクシア

#include <locale>
#include <codecvt>
#include <iostream>
#include <sstream>
// これをインクルードするとD3DXMATRIXのエラーは消えるがやりたくない
//#include <d3dx9.h>


/// <summary>
/// コンストラクタ
/// </summary>
My::Effect::Effect():
	CObject(4),
	m_sFilepas{},					// ファイルパス
	m_pos({ 0.0f, 0.0f, 0.0f }),	// 初期位置位置
	m_rot({ 0.0f, 0.0f, 0.0f }),	// 初期向き
	m_scl({ 1.0f, 1.0f, 1.0f }),	// 初期スケール
	m_col({ 1.0f, 1.0f, 1.0f, 1.0f}),
	m_nLoopCount(0),				// ループカウント
	m_bIsLoop(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
My::Effect::~Effect()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>S_OK:正常に初期化された</returns>
HRESULT My::Effect::Init()
{
	return S_OK;
}
/// <summary>
/// 終了処理
/// </summary>
void My::Effect::Uninit()
{
	if (g_effectRef != nullptr) {
	g_effectRef = nullptr;
}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void My::Effect::Update()
{
	My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();

	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();


	// 定期的にエフェクトを再生する
	if (m_bIsLoop ,	// ループするなら
		!(m_nLoopCount % m_nLoopInterval))	// カウントがインターバルを超えていたら
	{
		// 再生（カメラ正面あたりに配置）
		g_handle = managerRef->Play(g_effectRef, m_pos.x, m_pos.y, m_pos.z);
		managerRef->SetScale(g_handle, m_scl.x, m_scl.y, m_scl.z);
		managerRef->SetRotation(g_handle, m_rot.x, m_rot.y, m_rot.z);
		managerRef->SetAllColor(g_handle, Effekseer::Color(m_col.r * 255, m_col.g * 255, m_col.b * 255, m_col.a * 255));
	}

	// 再生カウントを進める
	m_nLoopCount++;

}

/// <summary>
/// 描画処理
/// </summary>
void My::Effect::Draw()
{

	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();   // デバイス取得

	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();
	EffekseerRenderer::RendererRef  rendererRef = pEffekseerManager->GetRenderer();

	if (managerRef == nullptr) return;

	// ビューとプロジェクションを毎フレーム更新（DX側でカメラ動く場合）
	Effekseer::Matrix44 proj, view;
	D3DXMATRIX dxView, dxProj;
	pDevice->GetTransform(D3DTS_VIEW, &dxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &dxProj);
	memcpy(&view, &dxView, sizeof(D3DXMATRIX));
	memcpy(&proj, &dxProj, sizeof(D3DXMATRIX));
	rendererRef->SetCameraMatrix(view);
	rendererRef->SetProjectionMatrix(proj);

	// Effekseer描画
	rendererRef->BeginRendering();
	managerRef->Draw();
	rendererRef->EndRendering();
}
/// <summary>
/// エフェクト読み込み
/// </summary>
/// <param name="sFilepas">ファイルパス</param>
/// <param name="pos">位置</param>
/// <returns>読み込み成功なら：true</returns>
bool My::Effect::Load(const std::string sFilepas, D3DXVECTOR3 pos)
{
	My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();

	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string u16s = convert.from_bytes(sFilepas);

	g_effectRef = Effekseer::Effect::Create(managerRef, u16s.c_str());
	if (g_effectRef == nullptr) {
		std::ostringstream oss;
		oss << "Effect::Load : ファイルパスの場所に指定のデータはありませんでした" << sFilepas << "\n";
		OutputDebugStringA(oss.str().c_str());
		return false;
	}

	// 再生（カメラ正面あたりに配置）
	g_handle = managerRef->Play(g_effectRef, pos.x, pos.y, pos.z);
	return true;
}
/// <summary>
/// 位置設定
/// </summary>
/// <param name="pos">設定する位置</param>
void My::Effect::SetPos(D3DXVECTOR3 pos)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトを移動する。
	m_pos = pos;
	managerRef->SetLocation(g_handle, m_pos.x, m_pos.y, m_pos.z);
}
/// <summary>
/// 向き設定(ラジアン)
/// </summary>
/// <param name="rot">設定する向き(ラジアン)</param>
void My::Effect::SetRot(D3DXVECTOR3 rot)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの向きを設定
	m_rot = rot;
	managerRef->SetRotation(g_handle, m_rot.x, m_rot.y, m_rot.z);
}
/// <summary>
/// スケール設定
/// </summary>
/// <param name="scl">設定するスケール</param>
void My::Effect::SetScl(D3DXVECTOR3 scl)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトの大きさを変更する。
	m_scl = scl;
	managerRef->SetScale(g_handle, m_scl.x, m_scl.y, m_scl.z);
}
/// <summary>
/// 色の設定
/// </summary>
/// <param name="col">設定する色</param>
void My::Effect::SetCol(D3DXCOLOR col)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	m_col = col;
	managerRef->SetAllColor(g_handle, Effekseer::Color(m_col.r * 255, m_col.g * 255, m_col.b * 255, m_col.a * 255));
}
/// <summary>
/// 位置の加算
/// </summary>
/// <param name="pos">移動分の距離</param>
void My::Effect::AddPos(D3DXVECTOR3 pos)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトを移動する。
	m_pos.x += pos.x;
	m_pos.y += pos.y;
	m_pos.z += pos.z;
	managerRef->SetLocation(g_handle, m_pos.x, m_pos.y, m_pos.z);
}
/// <summary>
/// 向き加算(ラジアン)
/// </summary>
/// <param name="rot">加算する向き(ラジアン)</param>
void My::Effect::AddRot(D3DXVECTOR3 rot)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの向きを設定
	m_rot += rot;
	managerRef->SetRotation(g_handle, m_rot.x, m_rot.y, m_rot.z);
}
/// <summary>
/// スケール加算
/// </summary>
/// <param name="scl">加算するスケール</param>
void My::Effect::AddScl(D3DXVECTOR3 scl)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトの大きさを変更する。
	m_scl.x += scl.x;
	m_scl.y += scl.y;
	m_scl.z += scl.z;
	managerRef->SetScale(g_handle, m_scl.x, m_scl.y, m_scl.z);
}
/// <summary>
/// 色の加算
/// </summary>
/// <param name="col">加算する色の量</param>
void My::Effect::AddCol(D3DXCOLOR col)
{
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	m_col += col;
	managerRef->SetAllColor(g_handle, Effekseer::Color(m_col.r * 255, m_col.g * 255, m_col.b * 255, m_col.a * 255));
}

// クリエイト
My::Effect* My::Effect::create(const std::string sFilepas)
{
	Effect* m_pEffect = new Effect();

	if (m_pEffect->Init())
	{
		m_pEffect->DeathFlag();
		return nullptr;
	}
	m_pEffect->Load(sFilepas, m_pEffect->m_scl);

	return m_pEffect;
}