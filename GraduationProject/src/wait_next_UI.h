//===========================================================================================================================================================
// 
// 次のUIヘッダー [wait_next_UI.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================
#ifndef _WAIT_NEXT_UI_H_
#define _WAIT_NEXT_UI_H_

#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトUIクラス
	 * @copydoc CObject2D
	 */
	class CWaitNextUI : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 25;	//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CWaitNextUI(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CWaitNextUI()override;

		/**
		 * @brief 初期化処理
		 * @return 成功したか
		 */
		HRESULT Init()override;

		/**
		 * @brief 終了処理
		 */
		void Uninit()override;

		/**
		 * @brief 更新処理
		 */
		void Update()override;

		/**
		 * @brief 描画処理
		 */
		void Draw()override;

		/**
		 * @brief 生成処理
		 * @param [in] 位置
		 * @return UIのポインタ
		 */
		static CWaitNextUI* Create(D3DXVECTOR3 pos);
	private:
	};
}
#endif