//===========================================================================================================================================================
// 
// カードの戦闘 [card_attack.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_BATTLE_ANIM_H_
#define _CARD_BATTLE_ANIM_H_
#include "main.h"
#include "card.h"
#include "card_deffence.h"
#include "pseudo_card_frame.h"
#include "pseudo_card_illust.h"
#include "card_frame.h"
namespace My
{
	class CCardBattleAnim
	{
	public:
		CCardBattleAnim();
		~CCardBattleAnim();

		void Uninit();

		bool DuelAnim();

		enum PSEND_CARD_TYPE
		{
			ATTACK = 0,
			DEFFENCE,
			MAX
		};
		struct PseundCard
		{
			CPsendCardFrame* card_frame[CCardFrame::FRAMETYPE_MAX];
			D3DXVECTOR3 card_pos = VEC3_RESET_ZERO;
			D3DXVECTOR3 card_move = VEC3_RESET_ZERO;
			std::string tex_pass;
		};

		void SetPsendCardFrame(CCard* card, PSEND_CARD_TYPE type);
		void DrawPasend();

		PseundCard m_pPseundCard[PSEND_CARD_TYPE::MAX];
		CPaseudoCardIllust* m_pCardIllust[PSEND_CARD_TYPE::MAX];
	};
}

#endif // !_CARD_ATK_H_