//================================
//
//フォントのマネージャー[font_manager]
//Author:towa matsuda
// 
//================================

#ifndef _FONT_MANAGER_H_ //このマクロ定義がされていなかったら
#define _FONT_MANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"
#include "font.h"

namespace My
{
	//エネミーマネージャーのクラス
	class CFontManager
	{
	public:

		struct StringData
		{
			const wchar_t* text;
			std::vector<CFont*> pFont;
		};

		//メンバ関数
		/**
		 * @brief コンストラクタ
		 */
		CFontManager();

		/**
		 * @brief デストラクタ
		 */
		~CFontManager();
		
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
		 * @brief 登録
		 * @param [in]font
		 * @param [in]一文字目のpos
		 * @param [in]テキストのサイズ
		 * @param [in]2文字以降テキストをずらす値
		 * @param [in]文字の太さ
		 * @param [in]フォント番号
		 */
		void Regist(const wchar_t* text,D3DXVECTOR3 first_pos, float size, float txt_shift, int thickness, int idx);

		/**
		 * @brief 削除
		 * @param [in]font
		 */
		void Release();

		/**
		 * @brief 削除
		 * @param [in]font
		 */
		void Remove(StringData font);

		//リストの取得
		std::list<StringData> GetList() { return m_apManager; }

		/**
		 * @brief 
		 * @param [in]idx
		 * @return 
		 */
		std::vector<CFont*> GetFont(int idx)
		{
			int i = 0;
			for (auto& itr : m_apManager)
			{
				if (i == idx) { return itr.pFont; }

				++i;
			}
		}

	private:

		//メンバ変数
		std::list<StringData> m_apManager;	//敵を管理
	};
}

#endif