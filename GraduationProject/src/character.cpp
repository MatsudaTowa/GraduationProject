//=============================================
//
//キャラクター処理[character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "character.h"
#include "colision.h"
#include "player.h"
#include "field.h"

//=============================================
//コンストラクタ
//=============================================
My::CCharacter::CCharacter(int nPriority) :CObjectX(nPriority),
m_PartsCnt(INT_ZERO),											//パーツ数
m_pMotionFrameCnt(nullptr),										//モーションのフレーム数
m_nKeySetCnt(INT_ZERO),											//キーの個数
m_Motion(INT_ZERO),												//モーション情報
m_isLoopFinish(),												//ループが終わったか
m_motion_data(),												//モーション設定
m_pShadow(),													//影のポインタ
m_ShadowSize(VEC3_RESET_ZERO)									//影のサイズ
{//イニシャライザーでプライオリティ設定、各メンバ変数初期化
}

//=============================================
//デストラクタ
//=============================================
My::CCharacter::~CCharacter()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CCharacter::Init()
{
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ GetPos().x,SHADOW_POS_Y,GetPos().z }, m_ShadowSize);

		//影のサイズ設定
		m_pShadow->SetSize({ m_ShadowSize });
	}

	if (m_pMotionFrameCnt == nullptr)
	{
		m_pMotionFrameCnt = new CCount;
		m_pMotionFrameCnt->SetCnt(INT_ZERO);
	}
	//最初どのモーションでもない値を代入
	m_Motion = -1;
	//ループモーション終わってる判定に
	m_isLoopFinish = true;

	//親クラスの初期化
	CObjectX::Init();

	//ワールドマトリックス設定
	ConversionMtxWorld();
	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CCharacter::Uninit()
{
	if (m_pMotionFrameCnt != nullptr)
	{
		delete m_pMotionFrameCnt;
		m_pMotionFrameCnt = nullptr;
	}
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
	for (int i = INT_ZERO; i < MAX_PARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			m_apModel[i]->Unload();
		}
	}

	CObjectX::Uninit();
}

//=============================================
//更新
//=============================================
void My::CCharacter::Update()
{
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetisDraw(GetisDraw());

		//影のサイズ設定
		m_pShadow->SetSize(m_ShadowSize);
		//影の位置設定
		m_pShadow->SetPos({ GetPos().x,SHADOW_POS_Y,GetPos().z });
	}

	for (int nCnt = 0; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->SetOldPos({ m_apModel[nCnt]->GetMtxWorld()._41,m_apModel[nCnt]->GetMtxWorld()._42,m_apModel[nCnt]->GetMtxWorld()._43 });
	}

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	//座標を更新
	SetPos(pos);
}

//=============================================
//描画
//=============================================
void My::CCharacter::Draw()
{
	CObjectX::Draw();
}

//=============================================
//モーション用の描画
//=============================================
void My::CCharacter::MotionDraw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DXMATRIX MtxWorld = GetMtxWorld();

	//マトリックスの初期化
	D3DXMatrixIdentity(&MtxWorld);

	//αテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	SetMtxWorld(MtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);


	for (int nCnt = INT_ZERO; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//=============================================
//パーツのロード
//=============================================
void My::CCharacter::Load_Parts(const char* FileName)
{
	//ファイルの読み込み
	std::ifstream File(FileName, std::ios::binary);

	//ファイルが開かなかったら関数を抜ける
	if (!File.is_open())
	{
		return;
	}

	File.read(reinterpret_cast<char*>(&m_motion_data), sizeof(CCharacter::MotionData));

	m_PartsCnt = m_motion_data.parts;

	for (int nCnt = 0; nCnt < m_motion_data.parts; ++nCnt)
	{
		if (m_apModel[nCnt] != nullptr) { continue; }
		m_apModel[nCnt] = CModel_Parts::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, &m_motion_data.path[nCnt][0]);
		m_apModel[nCnt]->SetIdx(m_motion_data.idx[nCnt]);
		m_apModel[nCnt]->SetIdxParent(m_motion_data.parent[nCnt]);

		//親を設定
		if (m_apModel[nCnt]->GetIdxParent() == -1)
		{
			m_apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			m_apModel[nCnt]->SetParent(m_apModel[m_apModel[nCnt]->GetIdxParent()]);
		}

		m_apModel[nCnt]->SetPos(m_motion_data.parts_pos[nCnt]);
		m_apModel[nCnt]->SetRot(m_motion_data.parts_rot[nCnt]);

		m_apModel[nCnt]->SetTPos(m_apModel[nCnt]->GetPos());
		m_apModel[nCnt]->SetTRot(m_apModel[nCnt]->GetRot());
	}

	File.close();
}

//=============================================
//モーション処理
//=============================================
void My::CCharacter::Motion()
{
	D3DXVECTOR3 MovePos[MAX_PARTS];
	D3DXVECTOR3 MoveRot[MAX_PARTS];

	int nNextKey = (m_nKeySetCnt + INT_ONE) % m_motion_data.motion_set[m_Motion].nNumKey;

	for (int nMotionCnt = INT_ZERO; nMotionCnt < GetNumParts(); nMotionCnt++)
	{
		D3DXVECTOR3 current_pos = m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].key[nMotionCnt].pos;
		D3DXVECTOR3 next_pos = m_motion_data.motion_set[m_Motion].keySet[nNextKey].key[nMotionCnt].pos;

		D3DXVECTOR3 current_rot = m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].key[nMotionCnt].rot;
		D3DXVECTOR3 next_rot = m_motion_data.motion_set[m_Motion].keySet[nNextKey].key[nMotionCnt].rot;

		float frame = (float)m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].nFrame;

		//現在との差を計算
		MovePos[nMotionCnt] = (next_pos - current_pos) / frame;
		MoveRot[nMotionCnt] = (next_rot - current_rot) / frame;
		D3DXVECTOR3 pos = m_apModel[nMotionCnt]->GetPos();
		D3DXVECTOR3 rot = m_apModel[nMotionCnt]->GetRot();

		pos += MovePos[nMotionCnt];
		rot += MoveRot[nMotionCnt];

		m_apModel[nMotionCnt]->SetPos({ pos.x,m_apModel[nMotionCnt]->GetPos().y,pos.z });
		m_apModel[nMotionCnt]->SetRot(rot);
	}

	if (m_pMotionFrameCnt->CountMeasure() > m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].nFrame)
	{

		m_pMotionFrameCnt->SetCnt(INT_ZERO);
		m_nKeySetCnt = (m_nKeySetCnt + INT_ONE) % m_motion_data.motion_set[m_Motion].nNumKey;
		if (m_nKeySetCnt == INT_ZERO)
		{
			if (m_motion_data.motion_set[m_Motion].nLoop == INT_ZERO)
			{
				//終わった判定
				m_isLoopFinish = true;
				return;
			}
			for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
			{
				m_apModel[nCntParts]->SetRot(m_motion_data.motion_set[m_Motion].keySet[0].key[nCntParts].rot);
			}
		}
	}
}

//=====================================
//引数で指定したモーションに切り替える
//=====================================
void My::CCharacter::SetMotion(int Motion)
{
	if (m_Motion == Motion)
	{//今のモーションと一致していたら
		//この関数を抜ける
		return;
	}

	m_Motion = Motion;

	//フレームリセット
	m_pMotionFrameCnt->SetCnt(INT_ZERO);

	//キーカウントリセット
	m_nKeySetCnt = INT_ZERO;

	if (m_motion_data.motion_set[m_Motion].nLoop == INT_ZERO)
	{
		//終わった判定
		m_isLoopFinish = false;
	}

	for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
	{
		m_apModel[nCntParts]->SetPos(m_apModel[nCntParts]->GetTPos());
		m_apModel[nCntParts]->SetRot(m_apModel[nCntParts]->GetTRot());

		m_apModel[nCntParts]->SetRot(m_motion_data.motion_set[Motion].keySet[0].key[nCntParts].rot);
	}
}

//=============================================
//ワールドマトリックスの変換
//=============================================
void My::CCharacter::ConversionMtxWorld()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DXMATRIX MtxWorld = GetMtxWorld();

	//マトリックスの初期化
	D3DXMatrixIdentity(&MtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

	for (int nCnt = INT_ZERO; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->ConversionMtxWorld();
	}
}