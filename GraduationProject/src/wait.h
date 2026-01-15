//===========================================================================================================================================================
// 
// 待機のヘッダー [wait.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _WAIT_H_
#define _WAIT_H_
#include "main.h"
#include "card.h"
#include "card_info_view.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 手札のクラス
	*/
	class CWait :public CCardInfoView
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CWait();

		/**
		* @brief デストラクタ
		*/
		~CWait() override;

		/**
		* @brief 初期化
		*/
		void Init() override;

		/**
		* @brief 更新
		*/
		void Update(CPlayerDuelState* state, CActiveSceneCharacter* player) override;

		/**
		* @brief 選択
		*/
		void Cast(CDuelCharacter* character, CActiveSceneCharacter* player)override;

		/**
		* @brief 生成
		*/
		static CWait* Create();

		/**
		* @brief 手札カードの取得
		*/
		//inline CCard** GetHandCard() { return &m_pCard[0]; }

		CCard* SearchList(CDuelCharacter* character, int num) override;

	private:
		D3DXVECTOR3 m_pos = VEC3_RESET_ZERO;
	};
}

#endif // !STAY_H_