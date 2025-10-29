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
	class CCard :public CObject3D
	{
	public:

		/**
		* @brief カードタイプの列挙型
		* @param 攻撃カード
		* @param 防御カード
		* @param 補助カード
		*/
		typedef enum
		{
			TYPE_ATTACK=0,
			TYPE_DEFFENCE,
			TYPE_ASSIST,
			TYPE_MAX
		}CARDTYPE_;

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
		 * @brief マウスでカードを選択する
		 * @param [out]カードが選択されたかどうかを返す
		 */
		bool CardSelectToMouse();

		/**
		 * @brief カードをマウスでキャストする
		 */
		void CardCastToMouse();

		/**
		 * @brief 生成
		 * @param [in]カードタイプ
		 */
		static CCard* Create(CCard::CARDTYPE_ type);

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

		/**
		* @brief カードタイプ取得
		*/
		inline void SetCardType(CCard::CARDTYPE_ type) { m_CardType = type; }
		inline CARDTYPE_ GetCardType() { return m_CardType; }

		/**
		* @brief ニュートラル位置
		*/
		inline void SetNeutralPos(D3DXVECTOR3 npos) { m_NeutralPos = npos; }
		inline D3DXVECTOR3 GetNeutralPos() { return m_NeutralPos; }

		/**
		* @brief ニュートラル位置
		*/
		inline void SetTarget(CInputMouse::AREA area) { m_target = area; }
		inline CInputMouse::AREA GetTarget() { return m_target; }

		//inline CCard* GetTop() { return m_pTop; }
		//inline CCard* GetPrev() { return m_pPrev; }

	private:
		//static CCard* m_pTop;	//先頭のオブジェクトポインタ
		//static CCard* m_pCur;	//現在（最後尾）のポインタ
		//CCard* m_pPrev;			//前のオブジェクトのポインタ
		//CCard* m_pNext;			//次のオブジェクトのポインタ

		int Cost;
		int AttackNum;

		/**
		 * @brief ステートをチェンジできるかどうか
		 */
		bool m_IsChange;

		/**
		* どのプレイヤーに対して攻撃するのか
		* TODO : AttackCardのみに記載
		*/
		CInputMouse::AREA m_target;

		/**
		 * @brief カードを選択しているか否か(trueだったら選択している)
		 */
		bool m_IsChoice;

		/**
		 * @brief カードカードのタイプ
		 */
		CARDTYPE_ m_CardType;

		/**
		 * @brief カードの状態
		 */
		CCardState* m_pState;

		/**
		 * @brief カードの状態列挙
		 */
		My::CCardState::CARD_STATE m_StateNum;

		/**
		 * @brief ニュートラル状態の位置(戻すため)
		 */
		D3DXVECTOR3 m_NeutralPos;

		D3DXVECTOR3 m_outpos;
	};
};

#endif // !CARD_H_