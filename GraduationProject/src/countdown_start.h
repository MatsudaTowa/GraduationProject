//=============================================
//
//開始時のカウントダウンを実行するクラス[countdown_start.h]
//Author Shimadu Yuto
//
//=============================================
#ifndef _COUNTDOWN_START_H_ //これが定義されてないとき

#define _COUNTDOWN_START_H_

/** @brief My 名前空間 */
namespace My
{
	/** @brief 開始時のカウントダウンクラス */
	class CCountdown_Start
	{
	public:

		//定数
		static constexpr int COUNT_NUM = 3;	//カウント数

		/**
		 * @brief コンストラクタ
		 */
		CCountdown_Start() : m_isStartCountdown(false), m_fCountStartTime(0.0f), m_fElapsedTime(0.0f), m_isEndCountDown(false), m_isCreateFirst(false){};

		/**
		 * @brief デストラクタ
		 */
		~CCountdown_Start() {};

		/**
		 * @brief 初期化処理
		 */
		virtual void Init() = 0;

		/**
	 * @brief 経過時間の更新
	 * @param 現在の更新に掛かった時間
	 */
		void Update();

		/**
		 * @brief カウント開始フラグの設定
		 * @param 開始時間
		 */
		inline void SetIsStartCountdown(bool start) { m_isStartCountdown = start; }

		/**
		 * @brief カウント開始フラグの取得
		 * @return カウント開始フラグ
		 */
		inline bool GetIsStartCountdown() { return m_isEndCountDown; }

		/**
		 * @brief カウント開始フラグの設定
		 * @param フラグ
		 */
		inline virtual void SetCountStartTime(float time) { m_fCountStartTime = time; }

		/**
		 * @brief 経過時間の設定
		 * @param 経過時間
		 */
		inline void SetElapsedTime(float time) { m_fElapsedTime = time; }

		/**
		 * @brief カウントダウンが終了フラグの取得
		 * @return カウントダウンが終了フラグ
		 */
		inline bool GetIsEndCountDown() { return m_isEndCountDown; }

		/**
		 * @brief UIの生成
		 * @param 現在の経過時間
		 * @param 過去の経過時間
		 */
		void CreateUI(float elapsedtime, float oldelapsedtime);

	private:
		
		//変数
		bool m_isStartCountdown;	//カウントダウン開始フラグ
		float m_fCountStartTime;	//カウントダウン開始の時刻
		float m_fElapsedTime;		//経過時間
		bool m_isEndCountDown;		//カウントダウンが終了したか
		bool m_isCreateFirst;		//最初のクリエイトを行ったか
	};
}
#endif