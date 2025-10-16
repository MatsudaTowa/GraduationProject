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
#include "card_state.h"

namespace My
{
	//class CCardState;

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
		 * @brief 生成
		 * @param [in]位置
		 */
		static CCard* Create();

		/**
		* @brief 状態を変更する
		*/
		void ChangeState(CCardState*state);
		void ChangeState(CCardState::CARD_STATE state);

		/**
		* @brief ステート取得
		*/
		inline CCardState* GetState() { return m_pState; }
		inline CCardState::CARD_STATE GetStateNum() { return m_StateNum; }

		//inline CCard* GetTop() { return m_pTop; }
		//inline CCard* GetPrev() { return m_pPrev; }

	private:
		//static CCard* m_pTop;	//先頭のオブジェクトポインタ
		//static CCard* m_pCur;	//現在（最後尾）のポインタ
		//CCard* m_pPrev;			//前のオブジェクトのポインタ
		//CCard* m_pNext;			//次のオブジェクトのポインタ


		/**
		 * @brief カードを選択しているか否か(trueだったら選択している)
		 */
		bool m_IsChoice;

		/**
		 * @brief カードの状態
		 */
		CCardState* m_pState;

		/**
		 * @brief カードの状態列挙
		 */
		My::CCardState::CARD_STATE m_StateNum;

		D3DXVECTOR3 m_outpos;
	};
};

#endif // !CARD_H_