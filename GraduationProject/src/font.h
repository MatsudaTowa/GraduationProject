//=============================================
//
//フォント表示[font.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _FONT_H_ //これが定義されてないとき

#define _FONT_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief フォントクラス
	 */
	class CFont: public CObject2D
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CFont(int nPriority = 28);

		/**
		 * @brief デストラクタ
		 */
		~CFont();

		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init();

		/**
		 * @brief 終了
		 */
		void Uninit();

		/**
		 * @brief 更新
		 */
		void Update();

		/**
		 * @brief 描画
		 */
		void Draw();
	private:

		// フォントビットマップを取得する関数
		BYTE* GetFontBitmap(HDC hDC, TCHAR c, GLYPHMETRICS& gm, int& bmpWidth, int& bmpHeight, int& level);
	};
}

#endif