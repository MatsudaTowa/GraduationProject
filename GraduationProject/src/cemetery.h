//===========================================================================================================================================================
// 
// 墓地のヘッダー [cemetery.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CEMETERY_H_
#define _CEMETERY_H_
#include "main.h"
#include "card.h"
#include "card_info_view.h"


/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 手札のクラス
	*/

	class CCemetery :public CCardInfoView
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CCemetery();

		/**
		* @brief デストラクタ
		*/
		~CCemetery() override;

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
		void Select(CPlayerDuelState* state) override;

		/**
		* @brief 選択
		*/
		void Cast(CDuelCharacter* character, CActiveSceneCharacter* player)override;

		/**
		* @brief 生成
		*/
		static CCemetery* Create();

		/**
		* @brief 手札カードの取得
		*/
		//inline CCard** GetHandCard() { return &m_pCard[0]; }

		CCard* SearchList(CDuelCharacter* character, int num) override;

	private:
	};
}

#endif // !HAND_H_