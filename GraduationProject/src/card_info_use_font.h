//=============================================
//
//カード情報の背景[card_info_usefont.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CARD_INFO_USEFONT_H_ //これが定義されてないとき

#define _CARD_INFO_USEFONT_H_
#include "main.h"
#include "object2D.h"
#include "font_manager.h"
#include "card_info_base.h"

namespace My
{
	//=============================================
	//カード情報の背景
	//=============================================
	class CCardInfoUseFont : public CCardInfoBase
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCardInfoUseFont(int nPriority);
		/**
		 * @brief デストラクタ
		 */
		~CCardInfoUseFont()override;
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