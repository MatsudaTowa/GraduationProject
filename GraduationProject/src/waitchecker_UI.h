//=============================================
//
//待機のチェッカー[waitchecker.h]
//Author Kasai Keisuke
//
//=============================================
#ifndef _WAITCHECKR_UI_H_	// これが定義されていないとき
#define _WAITCHECKR_UI_H_

#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトUIクラス
	 * @copydoc CObject2D
	 */
	class CWaitChecker_UI : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 21;	//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CWaitChecker_UI(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CWaitChecker_UI()override;

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
		static CWaitChecker_UI* Create(D3DXVECTOR3 pos);

		/**
		 * @brief 待機フラグの設定
		 * @param [in] bool
		 */
		void SetIsWaited(bool wait);

		/**
		 * @brief 待機フラグの取得
		 * @return フラグ
		 */
		inline bool GetIsWaited() { return m_isWaited; }

	private:

		//変数
		bool m_isWaited;	//待機状態か
	};
}

#endif