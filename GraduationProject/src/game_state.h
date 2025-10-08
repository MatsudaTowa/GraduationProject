//=============================================
//
//ゲームのステートパターン[game_state.h]
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
	class CGameState
	{
	public:
		CGameState() {};
		virtual ~CGameState() {};
		/**
		* @brief 通常処理(親では何もしない)
		* @param [in]ゲームポインタ
		*/
		virtual void Normal([[maybe_unused]] CGame* game) {};

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

	/** @brief 通常クラス */
	class CNormal :public CGameState
	{
	public:
		static constexpr int SPECIAL_TIME = 30;	//必殺技状態に移る時間
		/**
		* @brief ニュートラル処理
		* @param [in]ゲームプレイヤーポインタ
		*/
		void Normal(CGame* game) override;
	private:
	};

	/** @brief ポーズ状態処理クラス */
	class CPause :public CGameState
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
	class CCardCast :public CGameState
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
