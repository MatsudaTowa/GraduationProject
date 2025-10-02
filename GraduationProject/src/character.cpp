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
m_bLanding(false),												//着地しているか
m_move(VEC3_RESET_ZERO),										//移動量
m_nLife(INT_ZERO),												//体力
m_oldpos(VEC3_RESET_ZERO),										//過去の位置
m_PartsCnt(INT_ZERO),											//パーツ数
m_nMotionFrameCnt(INT_ZERO),									//モーションのフレーム数
m_nKeySetCnt(INT_ZERO),											//キーの個数
m_Motion(INT_ZERO),												//モーション情報
m_isLoopFinish(),												//ループが終わったか
m_Speed(),														//スピード
m_Jump(),														//ジャンプ力
m_motion_data(),													//モーション設定
m_nJumpCnt(INT_ZERO),											//ジャンプ数
m_pShadow(),													//影のポインタ
m_ShadowSize(VEC3_RESET_ZERO)									//影のサイズ
{//イニシャライザーでプライオリティ設定、各メンバ変数初期化
}

//=============================================
//デストラクタ
//=============================================
My::CCharacter::~CCharacter()
{
	for (int i = INT_ZERO; i < MAX_PARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			m_apModel[i]->Unload();
		}
	}
}

//=============================================
//初期化
//=============================================
HRESULT My::CCharacter::Init()
{
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ GetPos().x,SHADOW_POS_Y,GetPos().z }, m_ShadowSize);
	}

	//影のサイズ設定
	m_pShadow->SetSize({ m_ShadowSize });

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
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	CObjectX::Uninit();
}

//=============================================
//更新
//=============================================
void My::CCharacter::Update()
{
	m_pShadow->SetisDraw(GetisDraw());

	for (int nCnt = 0; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->SetOldPos({ m_apModel[nCnt]->GetMtxWorld()._41,m_apModel[nCnt]->GetMtxWorld()._42,m_apModel[nCnt]->GetMtxWorld()._43 });
	}

	//重力処理
	Gravity();

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	if (m_bLanding)
	{
		//移動量を更新(減速）
		m_move *= FLOAT_ONE - MOVE_FRICTION;
	}

	//過去の位置に今の位置を代入
	m_oldpos = pos;

	//移動量追加
	pos += m_move;
	//座標を更新
	SetPos(pos);

	//影のサイズ設定
	m_pShadow->SetSize(m_ShadowSize);

	//影の位置設定
	m_pShadow->SetPos({ GetPos().x,SHADOW_POS_Y,GetPos().z });

	//最大最小値取得
	D3DXVECTOR3 minpos = GetMinPos();
	D3DXVECTOR3 maxpos = GetMaxPos();

	if (m_bLanding)
	{//着地してるなら
		//ジャンプ数リセット
		m_nJumpCnt = INT_ZERO;
	}

	//床との接触処理
	HitField();
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

	m_Speed = m_motion_data.s_speed;
	m_Jump = m_motion_data.s_jump;
	m_PartsCnt = m_motion_data.s_parts;

	for (int nCnt = 0; nCnt < m_motion_data.s_parts; ++nCnt)
	{
		m_apModel[nCnt] = CModel_Parts::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, &m_motion_data.s_path[nCnt][0]);
		m_apModel[nCnt]->SetIdx(m_motion_data.s_idx[nCnt]);
		m_apModel[nCnt]->SetIdxParent(m_motion_data.s_parent[nCnt]);

		//親を設定
		if (m_apModel[nCnt]->GetIdxParent() == -1)
		{
			m_apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			m_apModel[nCnt]->SetParent(m_apModel[m_apModel[nCnt]->GetIdxParent()]);
		}

		m_apModel[nCnt]->SetPos(m_motion_data.s_parts_pos[nCnt]);
		m_apModel[nCnt]->SetRot(m_motion_data.s_parts_rot[nCnt]);

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

	int nNextKey = (m_nKeySetCnt + INT_ONE) % m_motion_data.s_motion_set[m_Motion].s_nNumKey;

	for (int nMotionCnt = INT_ZERO; nMotionCnt < GetNumParts(); nMotionCnt++)
	{
		MovePos[nMotionCnt] = (m_motion_data.s_motion_set[m_Motion].s_keySet[nNextKey].s_key[nMotionCnt].s_pos - m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_key[nMotionCnt].s_pos) / (float)m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame;
		MoveRot[nMotionCnt] = (m_motion_data.s_motion_set[m_Motion].s_keySet[nNextKey].s_key[nMotionCnt].s_rot - m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_key[nMotionCnt].s_rot) / (float)m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame;

		D3DXVECTOR3 pos = m_apModel[nMotionCnt]->GetPos();
		D3DXVECTOR3 rot = m_apModel[nMotionCnt]->GetRot();

		pos += MovePos[nMotionCnt];
		rot += MoveRot[nMotionCnt];

		m_apModel[nMotionCnt]->SetPos({ pos.x,m_apModel[nMotionCnt]->GetPos().y,pos.z });
		m_apModel[nMotionCnt]->SetRot(rot);
	}

	m_nMotionFrameCnt++;

	if (m_nMotionFrameCnt > m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame)
	{

		m_nMotionFrameCnt = INT_ZERO;
		m_nKeySetCnt = (m_nKeySetCnt + INT_ONE) % m_motion_data.s_motion_set[m_Motion].s_nNumKey;
		if (m_nKeySetCnt == INT_ZERO)
		{
			if (m_motion_data.s_motion_set[m_Motion].s_nLoop == INT_ZERO)
			{
				//終わった判定
				m_isLoopFinish = true;
				return;
			}
			for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
			{
				m_apModel[nCntParts]->SetRot(m_motion_data.s_motion_set[m_Motion].s_keySet[0].s_key[nCntParts].s_rot);
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
	m_nMotionFrameCnt = INT_ZERO;

	//キーカウントリセット
	m_nKeySetCnt = INT_ZERO;

	if (m_motion_data.s_motion_set[m_Motion].s_nLoop == INT_ZERO)
	{
		//終わった判定
		m_isLoopFinish = false;
	}

	for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
	{
		m_apModel[nCntParts]->SetPos(m_apModel[nCntParts]->GetTPos());
		m_apModel[nCntParts]->SetRot(m_apModel[nCntParts]->GetTRot());

		m_apModel[nCntParts]->SetRot(m_motion_data.s_motion_set[Motion].s_keySet[0].s_key[nCntParts].s_rot);
	}
}


//=============================================
//重力処理
//=============================================
void My::CCharacter::Gravity()
{
	if (m_move.y < GRAVITY_MAX)
	{
		m_move.y -= GRAVITY_MOVE;
	}
}

//=============================================
//ジャンプ処理
//=============================================
void My::CCharacter::Jump()
{
	m_move.y = m_Jump; //ジャンプ力代入
	m_bLanding = false; //空中状態
}

//=============================================
//床との接触判定
//=============================================
void My::CCharacter::HitField()
{
	D3DXVECTOR3 CharacterPos = GetPos();

	//サイズ取得
	D3DXVECTOR3 CharacterMin = GetMinPos();
	D3DXVECTOR3 CharacterMax = GetMaxPos();
	//敵との当たり判定
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = CObject::GetTopObject(i);	//先頭取得

		//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();	//次のポインタを取得

			//敵を見つけて速度を上げる
			if (pObj->GetType() == CObject::OBJECT_TYPE_FIELD)
			{
				CField* pField = dynamic_cast<CField*>(pObj);

				//床との当たり判定
				CColision::COLISION colision = CManager::GetInstance()->GetColision()->CheckColision_Y(m_oldpos, CharacterPos, CharacterMin, CharacterMax, pField->GetPos(), pField->GetSize());

				if (colision == CColision::COLISION::COLISON_TOP_Y)
				{//y(上)方向に当たってたら
					CharacterPos.y = m_oldpos.y;
					m_move.y = FLOAT_ZERO;
					m_bLanding = true; //着地
				}
				else
				{
					m_bLanding = false; //着地
				}

				if (m_oldpos.x > pField->GetPos().x - pField->GetSize().x
					&& CharacterPos.x <= pField->GetPos().x - pField->GetSize().x)
				{
					CharacterPos.x = m_oldpos.x;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.x < pField->GetPos().x + pField->GetSize().x
					&& CharacterPos.x >= pField->GetPos().x + pField->GetSize().x)
				{
					CharacterPos.x = m_oldpos.x;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.z > pField->GetPos().z - pField->GetSize().z
					&& CharacterPos.z <= pField->GetPos().z - pField->GetSize().z)
				{
					CharacterPos.z = m_oldpos.z;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.z < pField->GetPos().z + pField->GetSize().z
					&& CharacterPos.z >= pField->GetPos().z + pField->GetSize().z)
				{
					CharacterPos.z = m_oldpos.z;
					m_move.x = FLOAT_ZERO;
				}
				SetPos(CharacterPos);

			}
			pObj = pNext;							//ポインタを進める

		}
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