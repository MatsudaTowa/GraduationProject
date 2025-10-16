//=============================================
//
//エリア[center_area.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CENTER_AREA_H_ //これが定義されてないとき

#define _CENTER_AREA_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief エリアクラス
	 * @copydoc CObject2D_Triangle
	 */
	class CCenterArea : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 26;  //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CCenterArea(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CCenterArea()override;

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
		 * @return エリアのポインタ
		 */
		static CCenterArea* Create();
	private:
	};
}

#endif