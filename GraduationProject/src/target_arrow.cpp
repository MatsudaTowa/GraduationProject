//===========================================================================================================================================================
// 
// ターゲットアローの処理 [target_arrow.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "target_arrow.h"
#include "active_scene_manager.h"
#include "zone_manager.h"
#include "duel_manager.h"
#include "raknet.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\arrow_square.png";				// 矢印の四角の部分テクスチャ
	const std::string TRIANGLE_TEX_NAME = "data\\TEXTURE\\arrow_triangle.png";	// 矢印の三角の部分のテクスチャ
	const float MAX_SIZE = 150.0f;	// 矢印の最大長さ
	const float EXTEND_SPEED = 3.0f;	// 矢印が伸びる速さ
}

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CTargetArrow::CTargetArrow(int nPriority):CObject2D_Anim(nPriority),
m_target(VEC2_RESET_ZERO),
m_basesize(VEC2_RESET_ZERO),
m_attacker(VEC2_RESET_ZERO),
m_triangle(nullptr),
m_fLife(0.0f),
m_IsOverlapped(false),
m_ShiftPos(VEC2_RESET_ZERO),
m_TopCardList()
{
	m_TopCardList.clear();
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CTargetArrow::~CTargetArrow()
{
	m_TopCardList.clear();
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CTargetArrow::Init()
{
  	D3DXVECTOR3 pos = { m_attacker.x,m_attacker.y,0.0f };

	SetPos(pos);

	// サイズ設定
	SetSize({ 25.0f,0.0f });

	// ベースサイズ
	m_basesize = GetSize();

	// カラー設定
	SetColor(COLOR_WHITE);

	//テクスチャ登録
	CTexture* pTexture = GET_TEXTURE;
	CObject2D_Anim::BindTexture(pTexture->GetAddress(pTexture->Regist(TEX_NAME)));	// テクスチャ設定

	//頂点設定
	SetVtx();

	// 寿命を初期化
	//m_nLife = ONE_SECOND * 3;
	m_fLife = 3.0f;

	SetAnimFrame(1);

	CObject2D_Anim::Init();

	// 三角生成
	if (m_triangle == nullptr)
	{
		m_triangle = new CObject2D(5);
		m_triangle->BindTexture(pTexture->GetAddress(pTexture->Regist(TRIANGLE_TEX_NAME)));
		m_triangle->SetSize({ 50,50 });
		m_triangle->SetColor(COLOR_WHITE);
		m_triangle->SetVtx();
	}	

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CTargetArrow::Uninit()
{
	if (m_triangle != nullptr)
	{
		m_triangle->SetisDelete(true);
	}

	CObject2D_Anim::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CTargetArrow::Update()
{
	D3DXVECTOR2 size = GetSize();
	size.y += EXTEND_SPEED;
	SetSize(size);

	// 角度と長さを設定
	SetAngleLength();

	// 線上に位置を設定
	SetOnTheLinePos();

	// 寿命を減らす
	if (CRakNet::GetInstance()->GetOnline())
	{
		m_fLife -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();
	}
	else
	{
		m_fLife -= 0.017f;
	}
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CTargetArrow::Draw()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n %.3f", GetRot().z);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG

	CObject2D_Anim::Draw();
}

//===========================================================================================================
// 生成
//===========================================================================================================
My::CTargetArrow* My::CTargetArrow::Create(int attacker, int target, CCard* card)
{
	CTargetArrow* pTA = new CTargetArrow;

	// 矢印の角度を算出
	pTA->m_attacker = pTA->SetTargetPos(attacker,0);	// 攻撃者の位置
	pTA->m_target = pTA->SetTargetPos(target,1);		// 被攻撃者の位置

	pTA->Init(); 

	pTA->AddCardList(card);	//カードの位置を設定

	return pTA;
}

//===========================================================================================================
// カードの位置を設定
//===========================================================================================================
void My::CTargetArrow::SetCardPos(CCard* card)
{
	//守備以外のステイカードの数を取得
	int CardNum = 0;
	for (auto iter : m_TopCardList)
	{
		//守備カードは飛ばす
		if (!iter->IsSetUpStay()) continue;

		CardNum++;	//カード数の加算
	}

	//ワールド座標に変換
	D3DXVECTOR3 world_attackerpos = ConvertToWorldPoint(GET_CAMERA(GET_CAMERA_IDX), { m_attacker.x, m_attacker.y, 0.0f }, { 0.0f, 0.0f, 0.0f }); //発動者の位置
	D3DXVECTOR3 world_targetpos = ConvertToWorldPoint(GET_CAMERA(GET_CAMERA_IDX), { m_target.x, m_target.y, 0.0f }, { 0.0f, 0.0f, 0.0f }); //発動者の位置

	// 発動者と標的の座標の差
	float x = world_attackerpos.x - world_targetpos.x;
	float z = world_attackerpos.z - world_targetpos.z;

	// 二点の距離を求める
	float max_size = std::sqrt((x * x) + (z * z));

	//角度を求める
	float fAngle = atan2f(x, z);	// 設定

	//カード数から幅を算出
	float fWidth = max_size / (CardNum + 1);	//幅
	float fLength = fWidth * CardNum;			//長さ

	//置かれた順に配置
	for (auto& iter : m_TopCardList)
	{
		//守備カードは飛ばす
		if (!iter->IsSetUpStay()) continue;

		iter->SetPos({ fLength * sinf(fAngle + D3DX_PI) + world_attackerpos.x, 0.0f, fLength * cosf(fAngle + D3DX_PI) + world_attackerpos.z});
		fLength -= fWidth;	//幅の値だけ減らす
	}
}

//===========================================================================================================
// ターゲットの
//===========================================================================================================
D3DXVECTOR2 My::CTargetArrow::SetTargetPos(int targetnum, int type)
{
	// 今回のターゲットの位置
	D3DXVECTOR2 target = VEC2_RESET_ZERO;

	// 同じターゲットだった場合のずらす値
	float fShiftPos = 200.0f;

	switch (targetnum)
	{
	case CInputMouse::UP:
		target = arrow_up;
		break;

	case CInputMouse::DOWN:
		target = arrow_down;
		break;

	case CInputMouse::LEFT:
		target = arrow_left;
		break;

	case CInputMouse::RIGHT:
		target = arrow_right;
		break;
	}

	// 同じだったら位置を少し外側にずらす
	if (m_attacker == target)
	{
		switch (targetnum)
		{
		case CInputMouse::UP:
			target.y -= fShiftPos;
			break;

		case CInputMouse::DOWN:
			target.y += fShiftPos;
			break;

		case CInputMouse::LEFT:
			target.x -= fShiftPos;
			break;

		case CInputMouse::RIGHT:
			target.x += fShiftPos;
			break;
		}
	}

	return target;
}

//===========================================================================================================
// 線上に位置を設定する(線形補間)
//===========================================================================================================
void My::CTargetArrow::SetOnTheLinePos()
{
	// マウスの取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;
	D3DXVECTOR3 mousepos = pMouse->GetMousePos();

	// 倍率の係数
	float a = 0.5f;

	// 最大サイズ(発動者と標的の距離)
	float max_size = FLOAT_ZERO;

	// 発動者と標的の座標の差
	float x = m_attacker.x - m_target.x;
	float y = m_attacker.y - m_target.y;

	// 二点の距離を求める
	max_size = std::sqrt((x * x) + (y * y));

	// 原点が中心のため半分にする
	max_size *= HALF;

	// 位置・サイズの取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR2 size = GetSize();

	// 計算結果
	D3DXVECTOR2 result = { 0.0f,0.0f };
	D3DXVECTOR2 result2 = { 0.0f,0.0f };

	// 割合(サイズと最大サイズ)
	float ratio = size.y / max_size;

	// 線形補間
	result.x = std::lerp(m_attacker.x, m_target.x, ratio * a);
	result.y = std::lerp(m_attacker.y, m_target.y, ratio * a);

	// 位置の設定
	pos = { result.x + m_ShiftPos.x, result.y + m_ShiftPos.y, 0.0f };
	SetPos(pos);

	// 線形補間
	result2.x = std::lerp(pos.x, m_target.x + m_ShiftPos.x, ratio);
	result2.y = std::lerp(pos.y, m_target.y + m_ShiftPos.y, ratio);
	
	if (m_triangle != nullptr)
	{
		m_triangle->SetPos({ result2.x,result2.y,0.0f });
	}
	
	// 最大サイズまで伸びたら
	if (GetSize().y >= max_size)
	{
		// 最初のサイズに戻す
		SetSize(m_basesize);
	}
}

//===========================================================================================================
// 角度と長さを設定する
//===========================================================================================================
void My::CTargetArrow::SetAngleLength()
{
	CInputMouse* pMouse = GET_INPUT_MOUSE;
	D3DXVECTOR3 mousepos = pMouse->GetMousePos();

	// 角度の設定
	D3DXVECTOR3 rot = VEC3_RESET_ZERO;	// 初期化
	float angle = FLOAT_ZERO;	// 初期化
	angle = atan2f(GetSize().x,GetSize().y);	// 設定
	rot.z = atan2f(m_attacker.y - m_target.y, m_attacker.x - m_target.x);	// 設定
	rot.z += HALF_PI;
	rot.z *= -1;

	SetRot(rot);

	// サイズを代入
	float dx = GetSize().x;
	float dy = GetSize().y;

	// 長さを設定
	float length = FLOAT_ZERO;	// 初期化
	length = sqrtf(dx * dx + dy * dy);	// 設定

	//頂点設定
	SetVtx(angle, length);

	// TODO : これはこのクラスを使うべきかぁ
	if (m_triangle != nullptr)
	{
		// サイズを代入
		float tri_dx = m_triangle->GetSize().x;
		float tri_dy = m_triangle->GetSize().y;

		float tri_angle = atan2f(m_triangle->GetSize().x, m_triangle->GetSize().y);

		// 長さを設定
		float tri_length = FLOAT_ZERO;	// 初期化
		tri_length = sqrtf(tri_dx * tri_dx + tri_dy * tri_dy);	// 設定

		// 角度の設定
		D3DXVECTOR3 tri_rot = VEC3_RESET_ZERO;	// 初期化
		tri_rot.z = atan2f(m_target.y - m_attacker.y, m_target.x - m_attacker.x);	// 設定


		tri_rot.z += HALF_PI;
		tri_rot *= -1;
		m_triangle->SetRot(tri_rot);
		m_triangle->SetVtx(tri_angle, tri_length);
	}
}
