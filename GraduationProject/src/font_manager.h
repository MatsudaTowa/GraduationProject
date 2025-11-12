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
		 * @brief 
		 * @param [in]text
		 * @param [in]first_pos
		 * @param [in]size
		 * @param [in]txt_shift
		 * @param [in]thickness
		 * @param [in]idx
		 */
		void SetText(const wchar_t* text, D3DXVECTOR3 first_pos, float size, float txt_shift, int thickness, int idx);

		/**
		 * @brief 削除
		 * @param [in]font
		 */
		void Release();

		//リストの取得
		std::vector<CFont*> GetList() { return m_Font; }

	private:

		//メンバ変数
		std::vector<CFont*> m_Font;	//敵を管理
	};
}

#endif