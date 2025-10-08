//===========================================================================================================================================================
// 
// カードモデル [card.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_H_
#define _CARD_H_
#include "main.h"
#include "objectX.h"

namespace My
{
	class CCardState;

	class CCard :public CObjectX
	{
	public:

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCard(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCard()override;

		/**
		 * @brief 初期化
		 */
		HRESULT Init()override;

		/**
		 * @brief 終了
		 */
		void Uninit()override;

		/**
		 * @brief 更新
		 */
		void Update()override;

		/**
		 * @brief 描画
		 */
		void Draw()override;

		/**
		 * @brief 攻撃待機時間
		 */
		void StandbyAttack();

		/**
		 * @brief 生成
		 * @param [in]位置
		 */
		static CCard* Create();

		/**
		* @brief 状態を変更する
		*/
		void ChangeState(CCardState*state);

	private:
		/**
		 * @brief カードを選択しているか否か(trueだったら選択している)
		 */
		bool m_IsChoice;

		/**
		 * @brief カードの状態
		 */
		CCardState* m_pState;
	};
};

#endif // !CARD_H_