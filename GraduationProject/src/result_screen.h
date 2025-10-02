//=============================================
//
//リザルト画面[result_screen.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_SCREEN_H_ //これが定義されてないとき

#define _RESULT_SCREEN_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルト画面クラス
	 * @copydoc CObject2D
	 */
	class CResult_Screen : public CObject2D
	{
	public:
		static const int RESULT_SCREEN_PRIORITY = 20;  //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CResult_Screen(int nPriority = RESULT_SCREEN_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CResult_Screen()override;

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

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @return スクリーンのポインタ
		 */
		static CResult_Screen* Create(D3DXVECTOR3 pos);
	private:
	};
}

#endif