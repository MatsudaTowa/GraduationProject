//=============================================
//
//エネミー[enemy.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ENEMY_H_ //これが定義されてないとき

#define _ENEMY_H_
#include "main.h"
#include "character.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief プレイヤークラス
	 * @copydoc CCharacter
	 */
	class CEnemy :public CCharacter
	{
	public:
		static constexpr int PRIORITY = 8;		//!<描画順

		/** @brief モーションの種類列挙*/
		enum Motion_Type
		{
			MOTION_NEUTRAL = 0,
			MOTION_MOVE,
			MOTION_ATTACK,
			MOTION_MAX,
		};

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CEnemy(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CEnemy() override;

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

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]方向
		 * @return エネミーポインタ
		 */
		static CEnemy* Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
	private:
	};
}
#endif