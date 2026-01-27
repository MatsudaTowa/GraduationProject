//=============================================
//
//オンラインで開始時のカウントダウンを実行するクラス[countdown_start_online.h]
//Author Shimadu Yuto
//
//=============================================
#ifndef _COUNTDOWN_START_ONLINE_H_ //これが定義されてないとき
#define _COUNTDOWN_START_ONLINE_H_

#include "countdown_start.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 開始時のカウントダウンクラス */
	class CCountdown_Start_Online : public CCountdown_Start
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CCountdown_Start_Online();

		/**
		 * @brief デストラクタ
		 */
		~CCountdown_Start_Online() {}

		/**
		 * @brief カウント開始フラグの設定
		 * @param フラグ
		 */
		void SetCountStartTime(float time) override
		{ 
			CCountdown_Start::SetCountStartTime(time);
			SetElapsedTime(time);
		}

		/**
		 * @brief 初期化処理
		 */
		void Init() override;

	};
}
#endif