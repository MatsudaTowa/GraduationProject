//===========================================================================================================================================================
// 
// card_frame_use_font.cppのヘッダー [card_frame_use_font.h]
// Author : matsuda towa
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_USE_FONT_H_
#define _CARD_FRAME_USE_FONT_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"
#include "font_manager.h"
#include <format>


/** @brief My 名前空間 */
namespace My
{
	class CCardFrameUseFont :public CCardFrame
	{
	public:
		CCardFrameUseFont(int nPriority);
		~CCardFrameUseFont();
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;

		/**
		 * @brief フォントマネージャ取得
		 * @return 
		 */
		inline CFontManager* GetFontManager() { return m_pFontManager; }

		/**
		 * @brief オフセット取得
		 * @return m_offsetpos
		 */
		inline D3DXVECTOR3 GetOffSetPos() { return m_offsetpos; }

		/**
		 * @brief フォントマネージャ設定
		 * @param [in]font_manager
		 */
		inline void SetFontManager(CFontManager* font_manager) { m_pFontManager = font_manager; }

		/**
		 * @brief オフセット座標設定
		 * @param [in]offsetpos
		 */
		inline void SetOffSetPos(D3DXVECTOR3 offsetpos) { m_offsetpos = offsetpos; }
	private:
		CFontManager* m_pFontManager;
		D3DXVECTOR3 m_offsetpos;
	};
}
#endif