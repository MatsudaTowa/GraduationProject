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
#include "ready_UI.h"
#include "condition.h"
#include "player_UI.h"
#include "target_arrow_manager.h"
#include "zone_num_UI_manager.h"
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
		static constexpr int MAX_ENERGY = 99;				//!<エナジー最大値
		static constexpr int MAX_LIFE = 99;					//!<体力最大値

		struct Status
		{
			int deckSize;	//!<デッキの枚数
			int trash;		//!<墓地枚数
			int hand_num;		//!<手札枚数
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

		void CreateZoneNumUI();

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
		 * @brief HPが0になったら殺す処理
		 */
		void DeathRegist();

		void Regist(CCondition* condition);	//登録
		void Remove(CCondition* condition);	//削除

		/**
		 * @brief ダメージを受ける
		 * @param [in]受けるダメージ
		 */
		void ReceiveDamage(int damage);

		//リストの取得
		std::list<CCondition*> GetConditionList() { return m_ConditionList; }

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
		inline void SetDeckSize(int decksize)
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
		inline void SetHandNum(int hand)
		{
			m_status.hand_num = hand;
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
			if (energy > MAX_ENERGY)
			{
				m_status.energy = MAX_ENERGY;
				return;
			}
			m_status.energy = energy;
		}

		/**
		 * @brief プレイヤーUIの設定
		 * @param [in]プレイヤーUIのポインタ
		 */
		inline void SetPlayerUI(CPlayerUI* pPlayer)
		{
			m_pPlayerUI = pPlayer;
		}

		/**
		 * @brief エリアの代入
		 * @param [in]area
		 */
		inline void SetArea(CInputMouse::AREA area)
		{
			m_area = area;
		}

		/**
		 * @brief デッキの代入
		 * @param [in]deck
		 */
		inline void SetArea(std::list<int> deck)
		{
			m_Deck = deck;
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
		inline int GetDeckSize()
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
			return m_status.hand_num;
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
		 * @brief プレイヤーUIの取得
		 * @return [in]プレイヤーUIのポインタ
		 */
		inline CPlayerUI* GetPlayerUI()
		{
			return m_pPlayerUI;
		}

		inline CZoneNumUIManager* GetZoneManager()
		{
			return m_zoneNumUIManager;
		}

		inline CInputMouse::AREA GetArea()
		{
			return m_area;
		}

		/**
		 * @brief デッキの取得
		 * @return [in]デッキのリスト
		 */
		inline std::list<int>& GetDeck()
		{
			return m_Deck;
		}

		// 割合計算のため public から private に変更
		static constexpr int START_LIFE = 10;				//!<最初の体力
	private:
		static constexpr int START_DECK = 40;				//!<最初のデッキ枚数
		static constexpr int START_HAND = 3;				//!<最初の手札枚数
		static constexpr int START_ENERGY = 1;				//!<最初のエナジー

		/**
		 * @brief UIの位置設定
		 */
		void UpdateUI();

		/**
		 * @brief デッキの生成
		 */
		void CreateDeck();

		Status m_status;			//!ステータス

		CInputMouse::AREA m_area;	//!自分がどこのエリアか

		int m_playerIdx = -1;		//!自分自身のID(接続順)
		bool m_isHost = false;		//!ホストかどうか

		CZoneNumUIManager* m_zoneNumUIManager;

		CActiveSceneCharacterState* m_pState;	//!ステート
		std::list<CCondition*> m_ConditionList;	//コンディションリスト

		CPlayerUI* m_pPlayerUI;

		std::list<int> m_Deck;	//デッキ
	};
}
#endif