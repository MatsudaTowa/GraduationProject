//=============================================
//
//ゲームのステートパターン[active_scene_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "card_deffence.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CActiveScene;
	class CEnemy;

	/** @brief ゲームのステートクラス */
	class CActiveSceneState
	{
	public:
		CActiveSceneState() {};
		virtual ~CActiveSceneState() {};

		/**
		* @brief ロビー処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Lobby([[maybe_unused]] CActiveScene* game) {};


		/**
		* @brief デュエル処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Duel([[maybe_unused]] CActiveScene* game) {};

		/**
		* @brief ポーズ状態処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Pause([[maybe_unused]] CActiveScene* game) {};

		/**
		* @brief ポーズ状態処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void CardCast([[maybe_unused]] CActiveScene* game) {};

		/**
		* @brief 通信処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Connect([[maybe_unused]] CActiveScene* game) {};
	};

	/** @brief ロビークラス */
	class CLobby :public CActiveSceneState
	{
	public:
		static constexpr int SPECIAL_TIME = 30;	//必殺技状態に移る時間
		CLobby();
		~CLobby() override;
		/**
		* @brief ロビー処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Lobby(CActiveScene* game) override;
		/**
		 * @brief オフラインでデュエルシーンに切り替える処理
		 */
		void OfflineChangeToDuel();
		/**
		* @brief 通信処理
		* @param [in]ゲームポインタ
		*/
		void Connect(CActiveScene* game) override;

		//対戦開始の合図
		void SetBattleSign(bool sign) { m_isBattle = sign; }

	private:
		//正方形なのでサイズは統一
		static constexpr float FIELD_SIZE = 600.0f;

		static constexpr int NUM_PLAYER = 4;
		static constexpr float RADIUS = 300.0f;
		/**
		 * @brief 足りていないプレイヤーを生成
		 * @param [in]total
		 * @param [in]enemy
		 */
		void FillEmptyPlayer(int& total, std::list<My::CEnemy*>& enemy);
		/**
		 * @brief プレイヤー生成
		 * @param [in]pKeyboard
		 * @param [in]enemy
		 */
		void CreatePlayers(My::CInputKeyboard* pKeyboard, std::list<My::CEnemy*>& enemy);
		/**
		 * @brief オンライン用の処理
		 */
		void Online();
		/**
		 * @brief オンラインで敵を追加する処理
		 */
		void OnlineAddEnemy();
		/**
		 * @brief オンラインでデュエルシーンに切り替える処理
		 */
		void OnlineChangeToDuel();

		static int m_characterIdx; //ID TODO:サーバーから自身のID取得するから疑似的にオフラインで再現するためにstaticにしてます。
		bool m_isBattle = false;
	};

	/** @brief デュエルクラス */
	class CDuel :public CActiveSceneState
	{
	public:

		CDuel();
		~CDuel() override;
		static constexpr int SPECIAL_TIME = 30;	//必殺技状態に移る時間
		/**
		* @brief デュエル処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Duel(CActiveScene* game) override;
		/**
		* @brief 通信処理
		* @param [in]ゲームポインタ
		*/
		void Connect(CActiveScene* game) override;

	private:
		/**
			 * @brief 時計回りに配置
			 * @param [in]center
			 * @param [in]radius
			 */
		void ArrangePlayerClockwise(const D3DXVECTOR3 center, float radius);

		/**
		 * @brief キャラクターのエリアを判断
		 * @param [in]angle
		 * @return エリア
		 */
		CInputMouse::AREA CharacterArea(float angle);
	};

	/** @brief ポーズ状態処理クラス */
	class CPause :public CActiveSceneState
	{
	public:
		~CPause()override;
		/**
		* @brief ポーズ状態処理
		* @param [in]ゲームポインタ
		*/
		void Pause(CActiveScene* game) override;
		/**
		* @brief 通信処理
		* @param [in]ゲームポインタ
		*/
		void Connect(CActiveScene* game) override;
	private:
		static constexpr float THRESHOLD = 10.0f; // 距離が定数以下なら到達とする（必要に応じて調整）
	};

	/** @brief カードキャスト処理クラス */
	class CCardCast :public CActiveSceneState
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCardCast();
		/**
		 * @brief デストラクタ
		 */
		~CCardCast()override;
		/**
		* @brief カード状態処理
		* @param [in]ゲームポインタ
		*/
		void CardCast(CActiveScene* game) override;
		/**
		* @brief 通信処理
		* @param [in]ゲームポインタ
		*/
		void Connect(CActiveScene* game) override;
	private:
	};
}
#endif // !_GAME_PLAYER_STATE_H_
