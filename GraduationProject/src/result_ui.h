//=============================================
//
//リザルト[result_ui.h]
//Author Kasai Keisuke
//
//=============================================
#ifndef _RESULT_UI_H_	// これが定義されていないとき

#define _RESULT_UI_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief リザルトUIクラス 
	 * @copydoc CObject2D
	 */
	class CResult_Ui : public CObject2D
	{
	public:
		static const int RESULT_SCREEN_PRIORITY = 21;	//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CResult_Ui(int nPriority = RESULT_SCREEN_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CResult_Ui()override;

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
		static CResult_Ui* Create(D3DXVECTOR3 pos);
	};
}

#endif