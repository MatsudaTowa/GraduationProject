//===========================================================================================================================================================
// 
// 手札のヘッダー [hand.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _HAND_H_
#define _HAND_H_
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

	class CHand
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
		}HANDSTATE;

		/**
		* @brief [ 定数 ]
		* @param 開始時の手札
		* @param 手札の上限枚数
		*/
		static unsigned const int START_HANDS = 1;
		static unsigned const int MAX_HANDSCARD = 10;

		/**
		* @brief コンストラクタ
		*/
		CHand();

		/**
		* @brief デストラクタ
		*/
		~CHand();

		/**
		* @brief 初期化
		*/
		void Init();

		/**
		* @brief 開始時
		*/
		void Start(CPlayerDuelState* state);

		/**
		* @brief 更新
		*/
		void Update(CPlayerDuelState* state, CActiveSceneCharacter* player);

		/**
		* @brief 選択
		*/
		void Select(CDuelCharacter* character);

		/**
		* @brief 選択
		*/
		void Cast(CDuelCharacter* character, CActiveSceneCharacter* player);

		/**
		* @brief 手札を引く
		* @param [in]ドロー数
		*/
		void HandDraw(int drawnum, CPlayerDuelState* state);

		/**
		* @brief 手札のカードの位置設定
		*/
		void SetHandCardPos(CDuelCharacter* character);

		/**
		* @brief 生成
		*/
		static CHand* Create();

		/**
		* @brief カードが選択されているかどうか
		* @return 選択状態かのブール
		*/
		inline bool GetIsPickUp() { return m_IsPickUp; }

		/**
		* @brief 手札枚数の取得
		* @return 手札枚数
		*/
		inline int GetTotalNum() { return m_TotalNum; }

		/**
		* @brief 手札カードの取得
		*/
		//inline CCard** GetHandCard() { return &m_pCard[0]; }

		CCard* SearchHandList(int num);

	private:
		// 手札の状態
		HANDSTATE m_HandState;

		/**
		* @brief 手札のカードの状態変更
		*/
		void SelectStateSet(CDuelCharacter* character);

		/**
		* @brief 手札のカードの状態変更
		*/
		void DeleteCard(CDuelCharacter* character);
		
		/**
		* @brief カードタイプ別生成
		*/
		void SetCard(CCard::CARDTYPE_ type);

		//===========================================================================================================
		// メンバ変数
		//===========================================================================================================

		/**
		* @brief 手札の真ん中
		*/
		D3DXVECTOR3 m_CenterPos;

		/*
		* @brief 手札の配列
		* TODO : いったん配列で管理。のちにリスト構造などの構造で管理予定
		* HandList : 手札カード
		* StayList : ステイ中のカード
		* TriggerList : トリガーしたカード
		*/ 
		//My::CCard* m_pCard[MAX_HANDSCARD];

		std::list<CCard*>m_pHandList;

		/*My::CCard* m_pHandCard[MAX_HANDSCARD];
		My::CCard* m_pStayCard[MAX_HANDSCARD];
		My::CCard* m_pTriggerCard[MAX_HANDSCARD];*/

		/* @brief 現在持っているカード枚数 */
		int m_TotalNum;

		/* @brief 選択番号(手札を左から見た順番) */
		int m_SelectNum;

		/* @brief スタートを通ったかどうか */
		bool m_IsPassStart;

		/* @brief スタートを通ったかどうか */
		bool m_IsPickUp;
	};
}

#endif // !HAND_H_