//=============================================
//
//アクティブシーンキャラクター[achtive_scene_character.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ACTIVE_SCENE_CHARACTER_H_ //これが定義されてないとき

#define _ACTIVE_SCENE_CHARACTER_H_
#include "main.h"
#include "character.h"
#include "active_scene_character_state.h"
/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief アクティブシーンキャラクター[
	 * @copydoc CCharacter
	 */
	class CActiveSceneCharacter :public CCharacter
	{
	public:
		static constexpr int PRIORITY = 8;		//!<描画順

		struct Status
		{
			int deckSize;	//!<デッキの枚数
			int trash;		//!<墓地枚数
			int hand;		//!<手札枚数
			int energy;		//!<エナジー
			int life;		//!<体力
		};
		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CActiveSceneCharacter(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CActiveSceneCharacter() override;

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
		void ChangeState(CActiveSceneCharacterState* state);

		/**
		 * @brief ステート取得
		 * @return ステートポインタ
		 */
		inline CActiveSceneCharacterState* GetState()
		{
			return m_pState;
		}

		/**
		 * @brief プレイヤー番号代入
		 * @param [in]playerIdx
		 */
		inline void SetPlayerIdx(int playerIdx)
		{
			m_playerIdx = playerIdx;
		}

		/**
		* @brief ステータス設定
		* @param [in]ステータス
		*/
		inline void SetStatus(Status status)
		{
			m_status = status;
		}

		/**
		 * @brief デッキ枚数設定
		 * @param [in]デッキの枚数
		 */
		inline void SetDeck(int decksize)
		{
			m_status.deckSize = decksize;
		}

		/**
		 * @brief 墓地枚数設定
		 * @param [in]墓地枚数
		 */
		inline void SetTrash(int trash)
		{
			m_status.trash = trash;
		}

		/**
		 * @brief ハンド設定
		 * @param [in]ハンド枚数
		 */
		inline void SetHand(int hand)
		{
			m_status.hand = hand;
		}

		/**
		 * @brief 体力設定
		 * @param [in]体力
		 */
		inline void SetLife(int life)
		{
			m_status.life = life;
		}

		/**
		 * @brief エナジー設定
		 * @param [in]エナジー
		 */
		inline void SetEnergy(int energy)
		{
			m_status.energy = energy;
		}

		/**
		 * @brief 体力UIの設定
		 * @param [in]体力UIのポインタ
		 */
		inline void SetLifeUI(CLife_UI* plifeUI)
		{
			m_pLifeUI = plifeUI;
		}

		/**
		 * @brief エナジーUIの設定
		 * @param [in]エナジーUIのポインタ
		 */
		inline void SetEnergyUI(CEnergy_UI* pEnergyUI)
		{
			m_pEneryUI = pEnergyUI;
		}

		/**
		 * @brief ステータス取得
		 * @return ステータス
		 */
		inline Status GetStatus()
		{
			return m_status;
		}

		/**
		 * @brief デッキ枚数取得
		 * @return デッキ枚数
		 */
		inline int GetDeck()
		{
			return m_status.deckSize;
		}

		/**
		 * @brief 墓地枚数取得
		 * @return 墓地枚数
		 */
		inline int GetTrash()
		{
			return m_status.trash;
		}

		/**
		 * @brief ハンド取得
		 * @return ハンド枚数
		 */
		inline int GetHand()
		{
			return m_status.hand;
		}

		/**
		 * @brief 体力取得
		 * @return 体力
		 */
		inline int GetLife()
		{
			return m_status.life;
		}

		/**
		 * @brief エナジー取得
		 * @return エナジー
		 */
		inline int GetEnergy()
		{
			return m_status.energy;
		}

		/**
		 * @brief プレイヤーの番号取得
		 * @return idx
		 */
		inline int GetPlayerIdx()
		{
			return m_playerIdx;
		}

		/**
		 * @brief 体力UIの取得
		 * @return [in]体力UIのポインタ
		 */
		inline CLife_UI* GetLifeUI()
		{
			return m_pLifeUI;
		}

		/**
		 * @brief エナジーUIの取得
		 * @return [in]エナジーUIのポインタ
		 */
		inline CEnergy_UI* GetEnergyUI()
		{
			return m_pEneryUI;
		}
	private:
		static constexpr int START_DECK = 40;				//!<最初のデッキ枚数
		static constexpr int START_HAND = 3;				//!<最初の手札枚数
		static constexpr int START_LIFE = 10;				//!<最初の体力
		static constexpr int START_ENERGY = 1;				//!<最初のエナジー
		static constexpr int MAX_ENERGY = 99;				//!<エナジー最大値
		static constexpr int MAX_LIFE = 99;					//!<体力最大値

		Status m_status;			//!ステータス

		int m_playerIdx = -1;

		CLife_UI* m_pLifeUI;			//!<体力UI TODO:UIのインスタンスをキャラクターで持たせないように改良
		CEnergy_UI* m_pEneryUI;			//!エナジーUI TODO:UIのインスタンスをキャラクターで持たせないように改良
		CActiveSceneCharacterState* m_pState;	//!ステート
	};
}
#endif