//===========================================================================================================================================================
// 
// カードの戦闘 [card_attack.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#include "card_battle_anim.h"
//無名空間
namespace
{
	const float ILLUST_SHIFT_Y = -32.5f;
	const float SPEED = 15.0f;
}

//===========================================================================================================================================================
// 
//===========================================================================================================================================================
My::CCardBattleAnim::CCardBattleAnim():m_pCardIllust(),m_pPseundCard(), m_isBack(true), m_isDamage(false),m_isHit(false)
{
}

//===========================================================================================================================================================
// 
//===========================================================================================================================================================
My::CCardBattleAnim::~CCardBattleAnim()
{
}

void My::CCardBattleAnim::Uninit()
{
	for (int i = 0; i < PSEND_CARD_TYPE::MAX; ++i)
	{
		if (m_pCardIllust[i] != nullptr)
		{
			m_pCardIllust[i]->Uninit();
			m_pCardIllust[i] = nullptr;
		}

		for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
		{
			m_pPseundCard[i].card_frame[j]->Uninit();
			m_pPseundCard[i].card_frame[j] = nullptr;
		}
	}
}

//===========================================================================================================================================================
// 
//===========================================================================================================================================================
bool My::CCardBattleAnim::DuelAnim()
{
	DrawPasend();
	bool isbattle_finish = false;

	//プレイヤーの位置への方向情報
	D3DXVECTOR3 Vector = m_pPseundCard[DEFFENCE].card_pos  - m_pPseundCard[ATTACK].card_pos;

	// 目的地との距離を計算
	float distance = D3DXVec3Length(&Vector);
	//対象物との角度計算
	float angle = atan2f(Vector.x, Vector.z);
	for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
	{
		m_pPseundCard[ATTACK].card_frame[i]->SetisDraw(true);
		m_pPseundCard[DEFFENCE].card_frame[i]->SetisDraw(true);
	}
	if (distance > CCardFrame::CARD_WIDTH * 1.75f && !m_isHit)
	{
		D3DXVECTOR3 dir = Vector;
		D3DXVec3Normalize(&dir, &dir);

		D3DXVECTOR3 move = dir * SPEED;

		if (distance < CCardFrame::CARD_WIDTH * 5.75 && m_isBack)
		{
			m_pPseundCard[ATTACK].card_pos -= move;
			m_pPseundCard[ATTACK].card_move = move;
			m_pCardIllust[ATTACK]->SetPos(m_pCardIllust[ATTACK]->GetPos() - move);
		}
		else
		{
			m_isBack = false;
			m_pPseundCard[ATTACK].card_pos += move * 2.0f;
			m_pPseundCard[ATTACK].card_move = move * 2.0f;
			m_pCardIllust[ATTACK]->SetPos(m_pCardIllust[ATTACK]->GetPos() + move * 2.0f);
		}



		for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
		{
			m_pPseundCard[ATTACK].card_frame[i]->SetPos(m_pPseundCard[ATTACK].card_pos);
		}

		isbattle_finish = false;
	}
	else
	{
		m_isHit = true;
		D3DXVECTOR3 dir = Vector;
		D3DXVec3Normalize(&dir, &dir);

		float knockSpeed = SPEED * 2.0f;

		if (m_isDamage)
		{
			// 守備カードを正方向に吹き飛ばす
			D3DXVECTOR3 move = dir * knockSpeed;
			m_pPseundCard[ATTACK].card_move = VEC3_RESET_ZERO;

			m_pPseundCard[DEFFENCE].card_pos += move;
			m_pPseundCard[DEFFENCE].card_move = move;

			m_pCardIllust[DEFFENCE]->SetPos(m_pCardIllust[DEFFENCE]->GetPos() + move);

			for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
			{
				m_pPseundCard[DEFFENCE].card_frame[i]->SetPos(m_pPseundCard[DEFFENCE].card_pos);
			}

			// 画面外に出たら終了
			if (m_pPseundCard[DEFFENCE].card_pos.x - CCardFrame::CARD_WIDTH > SCREEN_WIDTH)
			{
				isbattle_finish = true;
			}
		}
		else
		{
			// 攻撃カードを逆方向に吹き飛ばす
			D3DXVECTOR3 move = -dir * knockSpeed;

			m_pPseundCard[ATTACK].card_pos += move;
			m_pPseundCard[ATTACK].card_move = move;

			m_pCardIllust[ATTACK]->SetPos(
				m_pCardIllust[ATTACK]->GetPos() + move
			);

			for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
			{
				m_pPseundCard[ATTACK].card_frame[i]->SetPos(m_pPseundCard[ATTACK].card_pos);
			}

			if (m_pPseundCard[ATTACK].card_pos.x+ CCardFrame::CARD_WIDTH < 0.0f)
			{
				isbattle_finish = true;
			}
		}
	}

	return isbattle_finish;
}

void My::CCardBattleAnim::SetPsendCardFrame(CCard* card, PSEND_CARD_TYPE type, bool isDamage)
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), card->GetPos()); //スクリーン座標に変換
	m_isDamage = isDamage;
	m_pPseundCard[type].card_pos = screen_pos;

	for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
	{
		m_pPseundCard[type].card_frame[i] = CPsendCardFrame::Create((CCardFrame::FRAMETYPE)i, m_pPseundCard[type].card_pos, VEC3_RESET_ZERO);
		if (i != CCardFrame::FRAMETYPE_ILLUST)
		{
			continue;
		}

		if (m_pCardIllust[type] == nullptr)
		{
			m_pCardIllust[type] = CPaseudoCardIllust::Create({ m_pPseundCard[type].card_pos.x,m_pPseundCard[type].card_pos.y + ILLUST_SHIFT_Y ,m_pPseundCard[type].card_pos.z }, { 35.0f,30.0f });
		}
	}
	m_pPseundCard[type].tex_pass = card->GetBaseStatus().imagePath;
}

void My::CCardBattleAnim::DrawPasend()
{
	for (int i = 0; i < PSEND_CARD_TYPE::MAX; ++i)
	{
		if (m_pCardIllust[i] != nullptr)
		{
			std::string get_tex_pass = m_pPseundCard[i].tex_pass;
			const std::string illust_folder = "data/TEXTURE/illust/";
			const std::string tex_pass = illust_folder + get_tex_pass;

			m_pCardIllust[i]->SetTexPath(tex_pass);
			m_pCardIllust[i]->SetisDraw(true);


			for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
			{
				if (j == CCardFrame::FRAMETYPE_ILLUST || j == CCardFrame::FRAMETYPE_COST || j == CCardFrame::FRAMETYPE_NAME || j == CCardFrame::FRAMETYPE_TEXT)
				{
					m_pPseundCard[i].card_frame[j]->SetColor(COLOR_WHITE);
					continue;
				}

				if (i == ATTACK)
				{
					m_pPseundCard[i].card_frame[j]->SetColor(COLOR_RED);
				}
				else
				{
					m_pPseundCard[i].card_frame[j]->SetColor(COLOR_BLUE);
				}
			}
		}
	}

}
