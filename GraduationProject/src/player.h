//=============================================
//
//プレイヤー[player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PLAYER_H_ //これが定義されてないとき

#define _PLAYER_H_
#include "main.h"
#include "character.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief プレイヤークラス
	 * @copydoc CCharacter
	 */	
	class CPlayer :public CCharacter
	{
	public:
		static const int PLAYER_LIFE = 150;			//!<体力
		static const int PLAYER_PRIORITY = 8;		//!<描画順

		static const D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<スポーン位置
		static const D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<スポーン方向
		static const float CORRECTION_VALUE_BELT;	//!<ベルトの補正値

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
		CPlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CPlayer() override;

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
		 * @param [in]プレイヤーポインタ
		 * @return プレイヤーポインタ
		 */
		static CPlayer* Create(CPlayer* player);

		/**
		 * @brief 方向のリセット
		 */
		void ResetRot();

		/**
		 * @brief 入力処理
		 */
		void Input();
	private:
		static const D3DXVECTOR3 SHADOW_SIZE;	 //!<影のサイズ

		//プレイヤーの移動関連
		static const float DAMPING_COEFFICIENT;		//!<移動抵抗
		static const int MAX_JUMPCNT;				//!<ジャンプ回数

		/**
		 * @brief リスポーン
		 */
		void ReSpawn();

		/**
		 * @brief プレイヤーのデバッグ表示
		 */
		virtual void Debug();
	};
}
#endif