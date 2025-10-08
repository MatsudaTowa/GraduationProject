//=============================================
//
//ゲーム処理[game.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_H_ //これが定義されてないとき

#define _GAME_H_
#include "main.h"
#include "scene.h"
#include "player.h"
#include "game_state.h"
#include "count.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief ゲームシーンクラス
	 * @copydoc CScene
	 */
	class CGame :public CScene
	{
	public:
		enum CAMERA_TYPE
		{
			GAME_CAMERA = 0,
			EVENT_CAMERA_000,
			EVENT_CAMERA_001,
			BOARD_GENERATE_CAMERA,
			CAMERA_MAX,
		};
		/**
		 * @brief コンストラクタ
		 */
		CGame();

		/**
		 * @brief デストラクタ
		 */	
		~CGame() override;

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
		 * @brief エリアの選択
		 */
		void SelectArea();

		/**
		 * @brief オブジェクトの更新を行うか決定
		 * @param [in]止めるかどうか
		 */
		void StopObject(bool isStop);

		/**
		 * @brief 描画
		 */
		void Draw() override;

		/**
		 * @brief 入力可能か取得
		 * @param 入力可能か
		 */
		inline bool GetPauseKey()
		{
			if (m_pPauseCnt == nullptr)
			{
				return false;
			}
			return m_pPauseCnt->CountUp();
		}

		/**
		 * @brief カウントリセット
		 */
		inline void ResetPauseCnt()
		{
			if (m_pPauseCnt == nullptr)
			{
				return;
			}
			m_pPauseCnt->SetCnt(0);
		}
	private:
		CCount* m_pPauseCnt;					//ポーズのキーを有効化するためのカウント
		CCount* m_pDelayCnt;					//リザルトに遷移するまでのディレイカウント
	};
}

#endif