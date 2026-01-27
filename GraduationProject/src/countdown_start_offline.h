//=============================================
//
//オフラインで開始時のカウントダウンを実行するクラス[countdown_start_offline.h]
//Author Shimadu Yuto
//
//=============================================
#ifndef _COUNTDOWN_START_OFFLINE_H_ //これが定義されてないとき
#define _COUNTDOWN_START_OFFLINE_H_

#include "countdown_start.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 開始時のカウントダウンクラス */
	class CCountdown_Start_Offline : public CCountdown_Start
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CCountdown_Start_Offline();

		/**
		 * @brief デストラクタ
		 */
		~CCountdown_Start_Offline() {}

		/**
		 * @brief 初期化処理
		 */
		void Init() override;

	};
}
#endif