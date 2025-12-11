//===========================================================================================================================================================
// 
// カードの情報を見るヘッダー [hand.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_INFO_VIEW_H_
#define _CARD_INFO_VIEW_H_
#include "main.h"
#include "card.h"


/** @brief My 名前空間 */
namespace My
{
	//================
	//前方宣言
	//================
	class CPlayerDuelState;
	class CDuelCharacter;
	class CActiveSceneCharacter;

	/**
	* @brief 手札のクラス
	*/

	class CCardInfoView
	{
	public:
		/**
		* @brief 手札の状態
		* これを使わないとキャスト中にキャストなどなるため
		* @param ニュートラル
		* @param 選択
		* @param キャスト
		*/
		typedef enum
		{
			NEUTRAL = 0,
			SELECT,
			CAST,
		}CARD_INFO_STATE;
		/**
		* @brief コンストラクタ
		*/
		CCardInfoView();

		/**
		* @brief デストラクタ
		*/
		virtual ~CCardInfoView();

		/**
		* @brief 初期化
		*/
		virtual void Init();

		/**
		* @brief 更新
		*/
		virtual void Update(CPlayerDuelState* state, CActiveSceneCharacter* player);

		/**
		* @brief 選択
		*/
		virtual void Select(CPlayerDuelState* state);

		/**
		* @brief 選択
		*/
		virtual void Cast(CDuelCharacter* character, CActiveSceneCharacter* player);

		/**
		* @brief 手札のカードの状態変更
		*/
		void SelectStateSet(CDuelCharacter* character);

		/**
		* @brief 手札カードの取得
		*/
		virtual CCard* SearchList(CDuelCharacter* character, int num) = 0;

		inline CARD_INFO_STATE GetInfoState() { return m_CardInfoState; }

		/**
		* @brief カードが選択されているかどうか
		* @return 選択状態かのブール
		*/
		inline bool GetIsPickUp() { return m_IsPickUp; }

		/**
		* @brief 枚数の取得
		* @return 手札枚数
		*/
		inline int GetTotalNum() { return m_TotalNum; }

		/**
		* @brief 選択番号取得
		* @return 選択番号
		*/
		inline int GetSelectNum() { return m_SelectNum; }

		inline void SetInfoState(CARD_INFO_STATE state) { m_CardInfoState = state; }

		/**
		 * @brief 選択されているかどうか設定
		 * @param [in]isPickUp
		 */
		inline void SetIsPickUp(bool isPickUp) { m_IsPickUp = isPickUp; }

		/**
		 * @brief 枚数の設定
		 * @param [in]total_num
		 */
		inline void SetTotalNum(int total_num) { m_TotalNum = total_num; }

		/**
		 * @brief 選択番号設定
		 * @param [in]isPickUp
		 */
		inline void SetSelectNum(int select_num) { m_SelectNum = select_num; }

	private:
		CARD_INFO_STATE m_CardInfoState;
		/* @brief 現在持っているカード枚数 */
		int m_TotalNum;

		/* @brief 選択番号 */
		int m_SelectNum;

		/* @brief スタートを通ったかどうか */
		bool m_IsPickUp;
	};
}

#endif // !HAND_H_