//============================================================================================
// 
// overlap_card.cpp のヘッダー [overlap_card.h]
// Author : Umeda Souma
// 
//============================================================================================
#ifndef _OVERLAP_CARD_
#define _OVERLAP_CARD_
#include "main.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class COverlapCard
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		COverlapCard();

		/**
		* @brief デストラクタ
		*/
		virtual ~COverlapCard();

		/**
		* @brief 登録
		* @param [in]pCard カードのポインタ
		*/
		void Regist(CCard* pCard);

		/**
		* @brief 削除
		*/
		void ReMove(CCard* pCard);

		/**
		* @brief カードを重ねる処理
		*/
		bool CheckOverlap(CCard* pCard);

		//==================================
		//			inline関数
		//==================================

		/**
		* @brief ターゲットの設定
		* @param [in]このカード群のターゲットを指し示すもの
		*/
		inline void SetTarget(CInputMouse::AREA target) { m_target = target; }

		/**
		* @brief ターゲットの取得
		* @return [out]このカード群のターゲットを指し示すもの
		*/
		inline CInputMouse::AREA GetTarget() { return m_target; }

		/**
		* @brief クリア
		*/
		inline void Clear() { m_pOverlapCards.clear(); }

		/**
		* @biref 重複しているカードのポインタ配列を取得
		* @return 重複しているカードのポインタ配列
		*/
		inline std::vector<CCard*> GetOverlapCards() { return m_pOverlapCards; }

		/**
		* @brief カードが重なったかどうかの設定
		* @param 重なったかどうか
		*/
		inline void SetIsOverlap(bool b) { m_IsOverlap = b; }

		/**
		* @biref カードが重なったかどうかの取得
		* @return 重なったかどうか
		*/
		inline bool GetIsOverlap() { return m_IsOverlap; }
	private:

		/**
		* @brief このカード群のターゲットを指し示すもの
		*/
		CInputMouse::AREA m_target;

		/**
		* @brief 重複しているカードのポインタ配列
		*/
		std::vector<CCard*> m_pOverlapCards;

		/**
		* @brief 重なったかどうか
		*/
		bool m_IsOverlap;
	};

	//====================================================================
	// 重なっている攻撃カード
	//====================================================================
	class COverlapAttackCard : public COverlapCard
	{
		// TODO
		// こうやってクラス分けするなら
		// オーバーライドしておいて、
		// AttackCard のみこのクラスの中に入れる等した方がいいかもしれない

	public:
		/**
		* @brief コンストラクタ
		*/
		COverlapAttackCard() {};

		/**
		* @brief デストラクタ
		*/
		~COverlapAttackCard()override {};
	};

	//====================================================================
	// 重なっている防御カード
	//====================================================================
	class COverlapDeffenceCard : public COverlapCard
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		COverlapDeffenceCard() {};

		/**
		* @brief デストラクタ
		*/
		~COverlapDeffenceCard()override {};
	};
}


#endif // !_OVERLAP_CARD_
