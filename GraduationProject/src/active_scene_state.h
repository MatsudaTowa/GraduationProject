//=============================================
//
//ゲームのステートパターン[active_scene_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CGame;

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
		virtual void Lobby([[maybe_unused]] CGame* game) {};


		/**
		* @brief デュエル処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Duel([[maybe_unused]] CGame* game) {};

		/**
		* @brief ポーズ状態処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Pause([[maybe_unused]] CGame* game) {};

		/**
		* @brief ポーズ状態処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void CardCast([[maybe_unused]] CGame* game) {};
	};

	/** @brief ロビークラス */
	class CLobby :public CActiveSceneState
	{
	public:
		static constexpr int SPECIAL_TIME = 30;	//必殺技状態に移る時間
		/**
		* @brief ロビー処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Lobby(CGame* game) override;
	private:
		//正方形なのでサイズは統一
		static constexpr float FIELD_SIZE = 200.0f;
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
		void Duel(CGame* game) override;
	private:
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
		void Pause(CGame* game) override;
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
		void CardCast(CGame* game) override;
	private:
	};
}
#endif // !_GAME_PLAYER_STATE_H_
