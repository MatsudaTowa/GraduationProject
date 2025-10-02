//=============================================
//
//タイトルのUI[title_screen.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_SCREEN_H_ //これが定義されてないとき

#define _TITLE_SCREEN_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief タイトル画面クラス
	 * @copydoc CObject2D
	 */
	class CTitle_Screen : public CObject2D
	{
	public:
		static const int TITLE_SCREEN_PRIORITY = 10;  //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CTitle_Screen(int nPriority = TITLE_SCREEN_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CTitle_Screen()override;

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
		static CTitle_Screen* Create(D3DXVECTOR3 pos);
	private:
	};
}

#endif