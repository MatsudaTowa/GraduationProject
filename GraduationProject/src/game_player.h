//=============================================
//
//プレイヤー[game_player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_H_ //これが定義されてないとき

#define _GAME_PLAYER_H_
#include "main.h"
#include "player.h"
#include "game_player_state.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief ゲームプレイヤークラス
	 * @copydoc CPlayer
	 */
	class CGamePlayer :public CPlayer
	{
	public:
		static const D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<スポーン位置
		static const D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<スポーン方向

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CGamePlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CGamePlayer() override;

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
		 * @brief BGMが切れた時の処理
		 */
		void Play_Sound_BGM_000();

		/**
		 * @brief ステート変更
		 * @param [in]次のステート
		 */
		void ChangeState(CGamePlayerState* state);
		
		/**
		 * @brief ステート取得
		 * @return ステートポインタ
		 */
		inline CGamePlayerState* GetState()
		{
			return m_pState;
		}

private:
		static const D3DXVECTOR3 SHADOW_SIZE;	 //!<影のサイズ

		/**
		 * @brief 入力処理
		 */
		void Input();

		/**
		 * @brief プレイヤーのデバッグ表示
		 */
		void Debug()override;

		CGamePlayerState* m_pState;						//!プレイヤーのステート
	};
}
#endif