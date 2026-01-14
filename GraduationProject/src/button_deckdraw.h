//===============================================================================================================
// 
// overlap_card_manager.cpp のヘッダー [overlap_card_manager.h]
// Author : Umeda Souma
// 
//===============================================================================================================
#ifndef _BUTTON_DECKDRAW_
#define _BUTTON_DECKDRAW_
#include "main.h"
#include "button.h"



namespace My
{
	class CPlayerDuelState;

	class CButtonDeckDraw : public CButton
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CButtonDeckDraw(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CButtonDeckDraw()override;
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init()override;
		/**
		 * @brief 終了
		 */
		void Uninit()override;
		/**
		 * @brief 更新
		 */
		void Update()override;
		/**
		 * @brief 描画
		 */
		void Draw()override;

		/**
		 * @brief マウスの判定
		 * @return 当たっているか
		 */
		bool ProcessMouseEvent(CPlayerDuelState* state);

		/**
		 * @brief ボタンが押された時の処理
		 */
		void ButtonTrigger()override;
		void ButtonTrigger(CPlayerDuelState* state);

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @return
		 */
		static CButtonDeckDraw* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	private:

		static const std::string TEX_NAME;

	};
}


#endif //_OVERLAP_CARD_MANAGER_