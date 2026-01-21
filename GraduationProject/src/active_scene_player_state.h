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
#include "wait.h"

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
		static constexpr float ENERGY_UP_FRAME = 3.0f;		//!<エナジーが上がるまでのフレーム数 TODO:今後のカードによっては変数に昇格するかも

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
		 * @brief 待機ゾーンの表示処理
		 * @param [in]zone
		 */
		void ViewWait(CWaitZone* zone) override;

		/**
		 * @brief エナジー増やすまでのカウント取得
		 * @return m_EnergyUpFrame
		 */
		/*inline CCount* GetEnergyCnt()
		{
			return m_pEnergyUpCount;
		}*/

		/**
		 * @brief エナジ増やすまでのカウント設定
		 * @param [in]nCnt
		 */
		/*inline void SetEnergyCnt(int nCnt)
		{
			if (m_pEnergyUpCount == nullptr) { return; }
			m_pEnergyUpCount->SetCnt(nCnt);
		}*/

		/**
		 * @brief エナジーのフレーム数取得
		 * @return m_EnergyUpFrame
		 */
		inline float GetEnergyFrame()
		{
			return m_fEnergyUpFrame;
		}

		/**
		 * @brief エナジーフレーム設定
		 * @param [in]frame
		 */
		inline void SetEnergyFrame(float frame)
		{
			//if (m_pEnergyUpCount == nullptr) { return; }
			m_fEnergyUpFrame = frame;
			//m_pEnergyUpCount->SetFrame(m_EnergyUpFrame);
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

		/**
		 * @brief 待機取得
		 * @return 待機
		 */
		inline CWait* GetWait()
		{
			return m_pWait;
		}

		/**
		 * @brief 待機設定
		 * @param [in]pWait
		 */
		inline void SetWait(CWait* pWait)
		{
			m_pWait = pWait;
		}

		inline bool GetIsWaitView()
		{
			return m_isWaitView;
		}

		inline void SetIsWaitView(bool isView)
		{
			m_isWaitView = isView;
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

		/**
		 * @brief カードを引く処理
		 */
		void DrawCard();

		float m_fEnergyUpCount;		//!エナジーを増やすまでのカウント
		float m_fEnergyUpCountRate;	//!エナジーを増やすまでのカウント
		float m_fEnergyUpFrame;		//!エナジーを増やすフレーム
		bool m_isWaitView;
		CHand* m_pHand;	// 手札
		CWait* m_pWait;
	};

}
#endif // !_GAME_PLAYER_STATE_H_
