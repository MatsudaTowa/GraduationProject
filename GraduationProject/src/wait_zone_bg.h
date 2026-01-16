//=============================================
//
//待機背景[wait_zone_bg.h]
//Author Kasai Keisuke
//
//=============================================
#ifndef _WAIT_ZONE_UI_H_	// これが定義されていないとき
#define _WAIT_ZONE_UI_H_

#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトUIクラス
	 * @copydoc CObject2D
	 */
	class CWaitZoneBG : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 11;	//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CWaitZoneBG(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CWaitZoneBG()override;

		/**
		 * @brief 初期化処理
		 * @return 成功したか
		 */
		HRESULT Init()override;

		/**
		 * @brief 終了処理
		 */
		void Uninit()override;

		/**
		 * @brief 更新処理
		 */
		void Update()override;

		/**
		 * @brief 描画処理
		 */
		void Draw()override;

		/**
		 * @brief 生成処理
		 * @param [in] 位置
		 * @return UIのポインタ
		 */
		static CWaitZoneBG* Create(D3DXVECTOR3 pos);

		/**
		 * @brief 待機フラグの設定
		 * @param [in] bool
		 */
		void SetIsWaited(bool wait);
	private:
	};
}

#endif