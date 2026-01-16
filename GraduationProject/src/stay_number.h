//===========================================================================================================================================================
// 
// stay_number.cppのヘッダー [stay_number.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _STAY_NUMBER_H_
#define _STAY_NUMBER_H_
#include "main.h"
#include "number.h"

// 名前空間
namespace My
{
	// クラス
	class CStayNumber :public CNumber_2D
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CStayNumber();

		/**
		* @brief デストラクタ
		*/
		~CStayNumber()override;

		/**
		* @brief 初期化
		* @return 成功したか
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

	private:
	};

}


#endif // !STAY_NUMBER_H_