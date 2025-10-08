//===========================================================================================================================================================
// 
// 手札のヘッダー [hand.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _HAND_H_
#define _HAND_H_
#include "main.h"


/** @brief My 名前空間 */
namespace My
{
	// 前方宣言
	class CCard;


	/**
	* @brief 手札のクラス
	*/
	class CHand
	{
	public:
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
		void Start();

		/**
		* @brief 更新
		*/
		void Update();

		/**
		* @brief 選択
		*/
		void Select();

		/**
		* @brief 手札を引く
		* @param [in]ドロー数
		*/
		void HandDraw(int drawnum);

		/**
		* @brief 生成
		*/
		static CHand* Create();

	private:
		/**
		* @brief 手札の真ん中
		*/
		D3DXVECTOR3 m_CenterPos;

		/**
		* @brief 手札のカードの位置設定
		*/
		void SetHandCardPos();

		//===========================================================================================================
		// メンバ変数
		//===========================================================================================================

		/*
		* @brief 手札の配列
		* TODO : いったん配列で管理。のちにリスト構造などの構造で管理予定
		*/ 
		My::CCard* m_pCard[MAX_HANDSCARD];

		/* @brief 現在持っているカード枚数 */
		int m_TotalNum;

		/* @brief 選択番号(手札を左から見た順番) */
		int m_SelectNum;

		/* @brief スタートを通ったかどうか */
		bool m_IsPassStart;
	};
}

#endif // !HAND_H_