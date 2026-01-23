//=============================================
//
//カードを選択出来る範囲[selsection_range.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _HAND_RANGE_H_ //これが定義されてないとき
#define _HAND_RANGE_H_
#include "main.h"
#include "selection_range.h"

namespace My
{
	//=============================================
	//カードを選択出来る範囲
	//=============================================
	class CHandRange : public CSelectionRange
	{
	public:
		static const int HAND_PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CHandRange(int nPriority = HAND_PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CHandRange()override;
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

#endif