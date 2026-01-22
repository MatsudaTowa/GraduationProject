//=============================================
//
//待機背景[wait_zone_bg.h]
//Author Kasai Keisuke
//
//=============================================
#ifndef _WAIT_ZONE_BG_H_	// これが定義されていないとき
#define _WAIT_ZONE_BG_H_

#include "button.h"
#include "pseudo_card_frame.h"
/** @brief My 名前空間 */
namespace My
{
	class CPsendCardFrame;
	/**
	 * @brief リザルトUIクラス
	 * @copydoc CObject2D
	 */
	class CWaitZoneBG : public CButton
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
		 * @brief ボタンが押された時の処理
		 */
		void ButtonTrigger() override;

		/**
		 * @brief マウスの判定
		 * @return 当たっているか
		 */
		bool ProcessMouseEvent() override;

		void CardisView();

		/**
		 * @brief 生成処理
		 * @param [in] 位置
		 * @return UIのポインタ
		 */
		static CWaitZoneBG* Create();
	private:

		static constexpr int NUM_CARD = 3;
		struct PseundCard
		{
			CPsendCardFrame* card_frame[CCardFrame::FRAMETYPE_MAX];
		};
		PseundCard m_pPseundCard[NUM_CARD];

		CCardFrame* m_pTopCardFrame[CCardFrame::FRAMETYPE_MAX];
	};
}

#endif