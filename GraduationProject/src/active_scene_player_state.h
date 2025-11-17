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
#include "hand.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CActiveScenePlayer;

	/** @brief プレイヤーのデュエルクラス */
	class CPlayerLobbyState :public CLobbyCharacter
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CPlayerLobbyState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CPlayerLobbyState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief プレイヤーのデュエルクラス */
	class CPlayerDuelState :public CDuelCharacter
	{
	public:
		static constexpr int ENERGY_UP_FRAME = 3 * 60;		//!<エナジーが上がるまでのフレーム数 TODO:今後のカードによっては変数に昇格するかも

		/**
		 * @brief コンストラクタ
		 */
		CPlayerDuelState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CPlayerDuelState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Duel(CActiveSceneCharacter* character) override;

		/**
		 * @brief エナジー増やすまでのカウント取得
		 * @return m_EnergyUpFrame
		 */
		inline CCount* GetEnergyCnt()
		{
			return m_pEnergyUpCount;
		}

		/**
		 * @brief エナジ増やすまでのカウント設定
		 * @param [in]nCnt
		 */
		inline void SetEnergyCnt(int nCnt)
		{
			if (m_pEnergyUpCount == nullptr) { return; }
			m_pEnergyUpCount->SetCnt(nCnt);
		}

		/**
		 * @brief エナジーのフレーム数取得
		 * @return m_EnergyUpFrame
		 */
		inline int GetEnergyFrame()
		{
			return m_EnergyUpFrame;
		}

		/**
		 * @brief エナジーフレーム設定
		 * @param [in]frame
		 */
		inline void SetEnergyFrame(int frame)
		{
			if (m_pEnergyUpCount == nullptr) { return; }
			m_EnergyUpFrame = frame;
			m_pEnergyUpCount->SetFrame(m_EnergyUpFrame);
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
		 * @brief UI生成
		 * @param [in]player
		 */
		void CreateDuelUI(CActiveScenePlayer* player);

		/**
		 * @brief エナジーを上げる処理
		 * @param [in]player
		 */
		void EnergyUp(CActiveScenePlayer* player);

		CCount* m_pEnergyUpCount;	//!エナジーを増やすまでのカウント
		int m_EnergyUpFrame;		//!エナジーを増やすフレーム
		CHand* m_pHand;	// 手札
	};

}
#endif // !_GAME_PLAYER_STATE_H_
