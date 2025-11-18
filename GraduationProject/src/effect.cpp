//============================================
//
// エフェクト[Effekseer.cpp]
// Author:Uedakou
// 
//============================================
#include "Effect.h"  // エフェクシア
#include "manager.h"  // マネージャー
#include "effect_manager.h"   // エフェクト

#include <locale>
#include <codecvt>
#include <iostream>
#include <sstream>

// これをインクルードするとD3DXMATRIXのエラーは消えるがやりたくない
//#include <d3dx9.h>



My::CEffect::Paramater::Paramater():
	m_sFilepas{ "data/Flame.efk" },					// ファイルパス
	m_pos({ 0.0f, 0.0f, 0.0f }),	// 初期位置位置
	m_rot({ 0.0f, 0.0f, 0.0f }),	// 初期向き
	m_scl({ 1.0f, 1.0f, 1.0f }),	// 初期スケール
	m_col({ 255, 255, 255, 255 }),	// 初期カラー
	m_nLoopInterval(300),			// ループのインターバル
	m_bIsLoop(false)				// ループするか
{
}
/// <summary>
/// コンストラクタ
/// </summary>
My::CEffect::CEffect():
	CObject(4),
	m_effectRef(nullptr),
	m_handle(0),
	m_nLoopCount(0)	// ループカウント
{
}
/// <summary>
/// デストラクタ
/// </summary>
My::CEffect::~CEffect()
{
}
/// <summary>
/// 初期化処理
/// </summary>
/// <returns>S_OK:正常に初期化された</returns>
HRESULT My::CEffect::Init()
{
	return S_OK;
}
/// <summary>
/// 終了処理
/// </summary>
void My::CEffect::Uninit()
{
	// ハンドル解放
	CManager* pMAnager = CManager::GetInstance();
	CEffectManager* pEffeksectManager = pMAnager->GetEffectManager();
	Effekseer::ManagerRef pManagerRef = pEffeksectManager->GetManager();
	pManagerRef->StopEffect(m_handle);

	// エフェクト解放
	if (m_effectRef != nullptr)
	{
		m_effectRef = nullptr;
	}
	//自分自身の解放
	Release();
}
/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void My::CEffect::Update()
{
	// マネージャー取得
	CManager* pManager = CManager::GetInstance();
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager();
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();


	// 定期的にエフェクトを再生する
	if (paramater.m_bIsLoop &&	// ループするなら
		!(m_nLoopCount % paramater.m_nLoopInterval))	// カウントがインターバルを超えていたら
	{
		if (managerRef != nullptr && m_effectRef != nullptr) {
			// 再生（カメラ正面あたりに配置）
			Play();
		}
	}
	// 再生カウントを進める
	m_nLoopCount++;

	// 寿命処理
	if (paramater.m_nLife !=-1 &&
		paramater.m_nLife > 0)
	{
		paramater.m_nLife--;
	}
	// 寿命が0なら死亡フラグを立てる
	if (paramater.m_nLife == 0)
	{
		//SetisDelete(true);
	}
}
/// <summary>
/// 描画処理
/// </summary>
void My::CEffect::Draw()
{
	// マネージャー取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();   // デバイス取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();		//
	EffekseerRenderer::RendererRef  rendererRef = pEffekseerManager->GetRenderer();

	// レンダラーが無かったら描画を抜ける
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
/// <param name="Filepas">ファイルパス</param>
/// <param name="pos">位置</param>
/// <returns>読み込み成功なら：true</returns>
bool My::CEffect::Load(_In_ const std::string Filepas)
{
	// マネージャーRef取得
	My::CEffectManager* pEffekseerManager = CManager::GetInstance()->GetEffectManager();
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();

	// ファイルパス確認
	if (Filepas.empty()) {
		std::cerr << "Error: File path is empty.\n";
		return false;
	}

	// UTF-8 → UTF-16 変換（Windows API使用）
	std::u16string u16s;
	int len = MultiByteToWideChar(CP_UTF8, 0, Filepas.c_str(), -1, nullptr, 0);
	if (len == 0) {
		std::cerr << "Error: MultiByteToWideChar length query failed.\n";
		return false;
	}

	std::vector<wchar_t> wide(len);
	if (MultiByteToWideChar(CP_UTF8, 0, Filepas.c_str(), -1, wide.data(), len) == 0) {
		std::cerr << "Error: MultiByteToWideChar conversion failed.\n";
		return false;
	}

	u16s.assign(wide.begin(), wide.end());

	// Effekseer::Effect::Create の例外対策
	try {
		m_effectRef = Effekseer::Effect::Create(managerRef, u16s.c_str());
	}
	catch (...) {
		std::cerr << "Error: Effekseer::Effect::Create threw an exception.\n";
		return false;
	}

	// 生成が成功していなかったら警告を出す
	if (m_effectRef == nullptr) {
		std::ostringstream oss;
		oss << "Effect::Load : 指定されたファイルパスにエフェクトデータが存在しません: " << Filepas << "\n";
		std::cerr << oss.str();
		return false;
	}

	return true;
}
/// <summary>
/// エフェクトを再生
/// </summary>
/// <returns>たエフェクトのハンドル</returns>
Effekseer::Handle My::CEffect::Play()
{
	// マネージャーRef取得
	My::CEffectManager* pEffekseerManager = CManager::GetInstance()->GetEffectManager();
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();

	// 再生
	if (managerRef == nullptr || m_effectRef == nullptr) {
		std::cerr << "Error: Manager or EffectRef is invalid.\n";
		return false;
	}

	m_handle = managerRef->Play(m_effectRef, paramater.m_pos.x, paramater.m_pos.y, paramater.m_pos.z);
	managerRef->SetScale(m_handle, paramater.m_scl.x, paramater.m_scl.y, paramater.m_scl.z);
	managerRef->SetRotation(m_handle, paramater.m_rot.x, paramater.m_rot.y, paramater.m_rot.z);
	managerRef->SetAllColor(m_handle, Effekseer::Color(paramater.m_col.R, paramater.m_col.G, paramater.m_col.B, paramater.m_col.A));

	return Effekseer::Handle();
}
/// <summary>
/// 色取得
/// </summary>
/// <returns></returns>
D3DXCOLOR My::CEffect::GetCol()
{

	return ColorSRGBToFloat(paramater.m_col);
}
/// <summary>
/// 位置設定
/// </summary>
/// <param name="pos">設定する位置</param>
void My::CEffect::SetPos(_In_ D3DXVECTOR3 pos)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトを移動する。

	paramater.m_pos = pos;
	managerRef->SetLocation(m_handle, paramater.m_pos.x, paramater.m_pos.y, paramater.m_pos.z);
}
/// <summary>
/// 向き設定(ラジアン)
/// </summary>
/// <param name="rot">設定する向き(ラジアン)</param>
void My::CEffect::SetRot(_In_ D3DXVECTOR3 rot)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの向きを設定
	paramater.m_rot = rot;
	managerRef->SetRotation(m_handle, paramater.m_rot.x, paramater.m_rot.y, paramater.m_rot.z);
}
/// <summary>
/// スケール設定
/// </summary>
/// <param name="scl">設定するスケール</param>
void My::CEffect::SetScl(_In_ D3DXVECTOR3 scl)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの大きさを変更する。
	paramater.m_scl = scl;
	managerRef->SetScale(m_handle, paramater.m_scl.x, paramater.m_scl.y, paramater.m_scl.z);
}
/// <summary>
/// スケール設定（一括）
/// </summary>
/// <param name="scl">設定するスケール</param>
void My::CEffect::SetScl(_In_ float scl)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの大きさを変更する。
	paramater.m_scl.x = scl;
	paramater.m_scl.y = scl;
	paramater.m_scl.z = scl;
	managerRef->SetScale(m_handle, paramater.m_scl.x, paramater.m_scl.y, paramater.m_scl.z);
}
/// <summary>
/// 色の設定
/// </summary>
/// <param name="col">設定する色</param>
void My::CEffect::SetCol(_In_ D3DXCOLOR col)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの色を変更
	paramater.m_col = ColorFloatToSRGB(col);

	managerRef->SetAllColor(m_handle, Effekseer::Color(paramater.m_col.R, paramater.m_col.G, paramater.m_col.B, paramater.m_col.A));
}
/// <summary>
/// 色の設定(SRGB)
/// </summary>
/// <param name="col">設定する色の量(SRGB)</param>
void My::CEffect::SetColorSRGB(_In_ Effekseer::Color col)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの色を変更
	paramater.m_col = col;

	managerRef->SetAllColor(m_handle, Effekseer::Color(paramater.m_col.R, paramater.m_col.G, paramater.m_col.B, paramater.m_col.A));
}
/// <summary>
/// 位置の加算
/// </summary>
/// <param name="pos">移動分の距離</param>
void My::CEffect::AddPos(_In_ D3DXVECTOR3 pos)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得


	// 再生中のエフェクトを移動する。
	paramater.m_pos.x += pos.x;
	paramater.m_pos.y += pos.y;
	paramater.m_pos.z += pos.z;
	managerRef->SetLocation(m_handle, paramater.m_pos.x, paramater.m_pos.y, paramater.m_pos.z);
}
/// <summary>
/// 向き加算(ラジアン)
/// </summary>
/// <param name="rot">加算する向き(ラジアン)</param>
void My::CEffect::AddRot(_In_ D3DXVECTOR3 rot)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの向きを変更する。
	paramater.m_rot += rot;
	managerRef->SetRotation(m_handle, paramater.m_rot.x, paramater.m_rot.y, paramater.m_rot.z);
}
/// <summary>
/// スケール加算
/// </summary>
/// <param name="scl">加算するスケール</param>
void My::CEffect::AddScl(_In_ D3DXVECTOR3 scl)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得


	// 再生中のエフェクトの大きさを変更する。
	paramater.m_scl.x += scl.x;
	paramater.m_scl.y += scl.y;
	paramater.m_scl.z += scl.z;
	managerRef->SetScale(m_handle, paramater.m_scl.x, paramater.m_scl.y, paramater.m_scl.z);
}
/// <summary>
/// スケール加算（一括）
/// </summary>
/// <param name="scl">加算するスケール</param>
void My::CEffect::AddScl(_In_ float scl)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得
	// 再生中のエフェクトの大きさを変更する。
	paramater.m_scl.x += scl;
	paramater.m_scl.y += scl;
	paramater.m_scl.z += scl;
	managerRef->SetScale(m_handle, paramater.m_scl.x, paramater.m_scl.y, paramater.m_scl.z);
}
/// <summary>
/// 色の加算(floatColor)
/// </summary>
/// <param name="col">加算する色の量(floatColor)</param>
void My::CEffect::AddColor(_In_ D3DXCOLOR col)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの色を変更
	Effekseer::Color EffCol = ColorFloatToSRGB(col);
	paramater.m_col.R += EffCol.R;
	paramater.m_col.G += EffCol.G;
	paramater.m_col.B += EffCol.B;
	paramater.m_col.A += EffCol.A;
	managerRef->SetAllColor(m_handle, Effekseer::Color(paramater.m_col.R, paramater.m_col.G, paramater.m_col.B, paramater.m_col.A));
}
/// <summary>
/// 色の加算(SRGB)
/// </summary>
/// <param name="col">加算する色の量(SRGB)</param>
void My::CEffect::AddColorSRGB(_In_ const std::array<int, 4>& col)
{
	// マネージャーRef取得
	My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
	My::CEffectManager* pEffekseerManager = pManager->GetEffectManager(); // エフェクシアマネージャー取得
	Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager(); // エフェクシアのマネージャー取得

	// 再生中のエフェクトの色を変更
	paramater.m_col.R += static_cast<uint8_t>(col[0]);
	paramater.m_col.G += static_cast<uint8_t>(col[1]);
	paramater.m_col.B += static_cast<uint8_t>(col[2]);
	paramater.m_col.A += static_cast<uint8_t>(col[3]);
	managerRef->SetAllColor(m_handle, Effekseer::Color(paramater.m_col.R, paramater.m_col.G, paramater.m_col.B, paramater.m_col.A));
}
/// <summary>
/// エフェクトの生成
/// </summary>
/// <param name="Filepas">生成するファイルパス</param>
/// <returns>生成したエフェクトのポインター</returns>
My::CEffect* My::CEffect::create(_In_ const std::string sFilepas)
{
	CEffect::Paramater param;

	std::ifstream inFile(sFilepas);
	if (!inFile) {
		std::cerr << "ファイルを開けませんでした\n";
		return nullptr;
	}

	std::string str0, str1, str2, str3;	// 文字列格納用
	std::string skip;		// スキップ用格納

	std::string line;
	while ((std::getline(inFile, line)))
	{
		std::istringstream iss(line);
		std::string key;
		if (!(iss >> key)) continue; // 空行スキップ

		if (key == "Filepas") {
			std::string eq;
			iss >> eq >> param.m_sFilepas; // '=' をスキップして読み込む
		}
		else if (key == "POS") {
			char eq;
			iss >> eq >> param.m_pos.x >> param.m_pos.y >> param.m_pos.z;
		}
		else if (key == "ROT") {
			char eq;
			iss >> eq >> param.m_rot.x >> param.m_rot.y >> param.m_rot.z;
		}
		else if (key == "SCL") {
			char eq;
			iss >> eq >> param.m_scl.x >> param.m_scl.y >> param.m_scl.z;
		}
		else if (key == "COL") {
			char eq;
			iss >> eq >> param.m_col.R >> param.m_col.G >> param.m_col.B >> param.m_col.A;
		}
		else if (key == "LOOP") {
			char eq;
			iss >> eq >> param.m_bIsLoop;
		}
		else if (key == "LOOP_INTEWRVAL") {
			char eq;
			iss >> eq >> param.m_nLoopInterval;
		}
		else if (key == "LIFE") {
			char eq;
			iss >> eq >> param.m_nLife;
		}
	}

	inFile.close();

	// 生成
	CEffect* m_pEffect = new CEffect();

	if (FAILED(m_pEffect->Init()))
	{
		m_pEffect->SetisDelete(true);
		return nullptr;
	}

	m_pEffect->Load(param.m_sFilepas);

	m_pEffect->SetPos(param.m_pos);
	m_pEffect->SetRot(param.m_rot);
	m_pEffect->SetScl(param.m_scl);
	m_pEffect->SetColorSRGB(param.m_col);
	m_pEffect->SetLoop(param.m_bIsLoop);
	m_pEffect->SetLoopInterval(param.m_nLoopInterval);
	m_pEffect->SetLife(param.m_nLife);

	m_pEffect->Play();	// エフェクトを再生

	return m_pEffect;
}
/// <summary>
/// D3DXCOLORからEffekseer::Colorへの変換
/// </summary>
/// <param name="value">変換する色</param>
/// <returns>変換した色</returns>
Effekseer::Color My::CEffect::ColorFloatToSRGB(_In_ const D3DXCOLOR col)
{
	Effekseer::Color EffColor;	// エフェクシアカラー格納用
	EffColor.R = static_cast<uint8_t>(col.r * 255.0f + 0.5f); // 四捨五入
	EffColor.G = static_cast<uint8_t>(col.g * 255.0f + 0.5f); // 四捨五入
	EffColor.B = static_cast<uint8_t>(col.b * 255.0f + 0.5f); // 四捨五入
	EffColor.A = static_cast<uint8_t>(col.a * 255.0f + 0.5f); // 四捨五入

	return EffColor;
}
/// <summary>
/// Effekseer::ColorからD3DXCOLORへの変換
/// </summary>
/// <param name="col">変換する色</param>
/// <returns>変換した色</returns>
D3DXCOLOR My::CEffect::ColorSRGBToFloat(_In_ const Effekseer::Color col)
{
	D3DXCOLOR EffColor;	// エフェクシアカラー格納用
	EffColor.r = static_cast<uint8_t>(col.R / 255.0f); // 四捨五入
	EffColor.g = static_cast<uint8_t>(col.G / 255.0f); // 四捨五入
	EffColor.b = static_cast<uint8_t>(col.B / 255.0f); // 四捨五入
	EffColor.a = static_cast<uint8_t>(col.A / 255.0f); // 四捨五入
	return EffColor;
}
/// <summary>
/// UTF-16変換の型整合性
/// </summary>
/// <param name="utf8"></param>
/// <returns></returns>
std::u16string My::CEffect::ConvertToUtf16(_In_ const std::string& utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	std::wstring wide(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wide[0], len);
	return std::u16string(wide.begin(), wide.end());
}