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
#include "hand.h"


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
		static constexpr D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<スポーン位置
		static constexpr D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<スポーン方向

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

		/**
		 * @brief 手札取得
		 * @return 手札
		 */
		inline CHand* GetHand()
		{
			return m_pHand;
		}

		/**
		 * @brief 手札設定
		 * @param [in]pHand
		 */
		inline void SetHand(CHand* pHand)
		{
			m_pHand = pHand;
		}

private:
		/**
		 * @brief 入力処理
		 */
		void Input();

		/**
		 * @brief プレイヤーのデバッグ表示
		 */
		void Debug()override;

		CGamePlayerState* m_pState;						//!プレイヤーのステート
		CHand* m_pHand;	// 手札
	};
}
#endif