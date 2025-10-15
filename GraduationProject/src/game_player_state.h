//=============================================
//
//ゲームプレイヤーのステートパターン[game_player_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_STATE_H_
#define _GAME_PLAYER_STATE_H_

#include "character.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CGamePlayer;

	/** @brief プレイヤーのステートクラス */
	class CGamePlayerState
	{
	public:
		CGamePlayerState() {};
		virtual ~CGamePlayerState() {};
		/**
		* @brief ロビー処理(親では何もしない)
		* @param [in]ゲームプレイヤーポインタ
		*/
		virtual void Lobby([[maybe_unused]] CGamePlayer* player) {};
		/**
		* @brief デュエル処理(親では何もしない)
		* @param [in]ゲームプレイヤーポインタ
		*/
		virtual void Duel([[maybe_unused]] CGamePlayer* player) {};
	};

	/** @brief プレイヤーのデュエルクラス */
	class CLobbyState :public CGamePlayerState
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CLobbyState();
		/**
		 * @brief デストラクタ
		 */
		~CLobbyState() override;
		/**
		* @brief デュエル処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Lobby(CGamePlayer* player) override;
	};

	/** @brief プレイヤーのデュエルクラス */
	class CDuelState :public CGamePlayerState
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CDuelState();
		/**
		 * @brief デストラクタ
		 */
		~CDuelState() override;
		/**
		* @brief デュエル処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Duel(CGamePlayer* player) override;
	private:
		static constexpr int ENERGY_UP_FRAME = 3 * 60;		//!<エナジーが上がるまでのフレーム数 TODO:今後のカードによっては変数に昇格するかも

		/**
		 * @brief UI生成
		 * @param [in]player
		 */
		void CreateDuelUI(CGamePlayer* player);

		/**
		 * @brief エナジーを上げる処理
		 * @param [in]player
		 */
		void EnergyUp(CGamePlayer* player);

		CCount* m_pEnergyUpCount;	//!エナジーを増やすまでのカウント
	};
}
#endif // !_GAME_PLAYER_STATE_H_
