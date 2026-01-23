//===========================================================================================================================================================
// 
// 次のUIヘッダー [wait_next_illust.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================
#ifndef _WAIT_NEXT_ILLUST_H_
#define _WAIT_NEXT_ILLUST_H_

#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトUIクラス
	 * @copydoc CObject2D
	 */
	class CWaitNextIllust : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 25;	//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CWaitNextIllust(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CWaitNextIllust()override;

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
		static CWaitNextIllust* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size);
	private:
	};
}
#endif