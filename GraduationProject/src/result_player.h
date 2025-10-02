//=============================================
//
//プレイヤー[result_player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_PLAYER_H_ //これが定義されてないとき

#define _RESULT_PLAYER_H_
#include "main.h"
#include "player.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトプレイヤークラス
	 * @copydoc CPlayer
	 */
	class CResultPlayer :public CPlayer
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CResultPlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CResultPlayer() override;

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
	};
}
#endif