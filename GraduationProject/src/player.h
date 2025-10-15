//=============================================
//
//プレイヤー[player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PLAYER_H_ //これが定義されてないとき

#define _PLAYER_H_
#include "main.h"
#include "active_scene_character.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief プレイヤークラス
	 * @copydoc CCharacter
	 */	
	class CPlayer :public CActiveSceneCharacter
	{
	public:
		static constexpr int PLAYER_PRIORITY = 8;		//!<描画順

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
		 * @param [in]pos
		 * @param [in]rot
		 * @param [in]id
		 * @return プレイヤーポインタ
		 */
		static CPlayer* Create(CPlayer* player, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int id);

		/**
		 * @brief 方向のリセット
		 */
		void ResetRot();

		/**
		 * @brief 入力処理
		 */
		void Input();
	private:
		static constexpr float SHADOW_SIZE_X = 20.0f;	 //!<影のサイズ
		static constexpr float SHADOW_SIZE_Z = 20.0f;	 //!<影のサイズ

		/**
		 * @brief プレイヤーのデバッグ表示
		 */
		virtual void Debug();
	};
}
#endif