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
		 * @param [in]番号
		 * @return フォントマネージャ
		 */
		inline CFontManager* GetFontManager(unsigned int idx)
		{
			if (idx >= m_FontManagerVector.size())
			{
				return nullptr;
			}
			return m_FontManagerVector[idx];
		}

		/**
		 * @brief 指定されたフォントマネージャを削除
		 * @param [in]番号
		 */
		inline void EraseFontManager(unsigned int idx)
		{
			if (idx >= m_FontManagerVector.size())
			{
				return;
			}
			CFontManager* font_manager = m_FontManagerVector[idx];
			font_manager->Uninit();
			delete font_manager;
			font_manager = nullptr;

			m_FontManagerVector.erase(m_FontManagerVector.begin() + idx);
		}

		/**
		 * @brief 追加
		 * @param [in]追加するポインタ
		 */
		inline void PushFontManager(CFontManager* font_manager)
		{
			if (font_manager == nullptr)
			{
				return;
			}
			m_FontManagerVector.push_back(font_manager);
		}

		/**
		 * @brief オフセット取得
		 * @return m_offsetpos
		 */
		inline D3DXVECTOR3 GetOffSetPos() { return m_offsetpos; }

		/**
		 * @brief オフセット座標設定
		 * @param [in]offsetpos
		 */
		inline void SetOffSetPos(D3DXVECTOR3 offsetpos) { m_offsetpos = offsetpos; }
	private:
		std::vector<CFontManager*> m_FontManagerVector;
		D3DXVECTOR3 m_offsetpos;
	};
}

#endif