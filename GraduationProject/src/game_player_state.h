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
		* @brief ニュートラル処理(親では何もしない)
		* @param [in]ゲームプレイヤーポインタ
		*/
		virtual void Neutral([[maybe_unused]] CGamePlayer* player) {};
	};

	/** @brief プレイヤーのニュートラルクラス */
	class CNeutralState :public CGamePlayerState
	{
	public:
		/**
		* @brief ニュートラル処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Neutral(CGamePlayer* player) override;
	};
}
#endif // !_GAME_PLAYER_STATE_H_
