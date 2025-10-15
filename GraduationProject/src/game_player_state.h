//=============================================
//
//ゲームプレイヤーのステートパターン[game_player_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_STATE_H_
#define _GAME_PLAYER_STATE_H_

#include "character.h"
#include "active_scene_character_state.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CGamePlayer;

	/** @brief プレイヤーのデュエルクラス */
	class CLobbyState :public CActiveSceneCharacterState
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
		* @param [in]キャラクターポインタ
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief プレイヤーのデュエルクラス */
	class CDuelState :public CActiveSceneCharacterState
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
		* @param [in]キャラクターポインタ
		*/
		void Duel(CActiveSceneCharacter* character) override;
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
