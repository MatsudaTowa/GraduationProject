//=============================================
//
//リザルト[result.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_H_ //これが定義されてないとき

#define _RESULT_H_
#include "main.h"
#include "scene.h"
#include "manager.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトクラス
	 * @copydoc CScene
	 */
	class CResult :public CScene
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CResult();

		/**
		 * @brief デストラクタ
		 */
		~CResult() override;

		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init() override;

		/**
		 * @brief 終了
		 */
		void Uninit() override;

		/**
		 * @brief 更新
		 */
		void Update() override;

		/**
		 * @brief 描画
		 */
		void Draw() override;
	private:
	};
}

#endif