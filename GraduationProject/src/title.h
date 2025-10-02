//=============================================
//
//タイトル処理[title.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_H_ //これが定義されてないとき

#define _TITLE_H_
#include "main.h"
#include "scene.h"
#include "player.h"
#include "title_screen.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief タイトルクラス
	 * @copydoc CScene
	 */
	class CTitle :public CScene
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CTitle();

		/**
		 * @brief デストラクタ
		 */
		~CTitle() override;

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

		CPlayer* m_pPlayer;				//!<プレイヤー
	};
}

#endif