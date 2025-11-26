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
		enum TextAlign
		{
			RIGHT = 0,
			CENTER,
			LEFT
		};

		//メンバ関数
		/**
		 * @brief コンストラクタ
		 */
		CFontManager();

		/**
		 * @brief 引数付きコンストラクタ
		 * @param [in]align
		 */
		CFontManager(TextAlign align);

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
		 * @param [in]フォントを表示するエリア
		 * @param [in]テキストのサイズ
		 * @param [in]2文字以降テキストをずらす値
		 * @param [in]文字の太さ
		 * @param [in]フォント番号
		 */
		void Regist(const wchar_t* text,D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col);

		/**
		 * @brief 登録(フォントサイズや間隔を自動調整)
		 * @param [in]font
		 * @param [in]一文字目のpos
		 * @param [in]フォントを表示するエリア
		 * @param [in]テキストのサイズ
		 * @param [in]2文字以降テキストをずらす値
		 * @param [in]文字の太さ
		 * @param [in]フォント番号
		 * @param [in]改行するかどうか
		 */
		void RegistAdjustFontSize(const wchar_t* text,D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col,bool isLinebrake);

		/**
		 * @brief 
		 * @param [in]font
		 * @param [in]一文字目のpos
		 * @param [in]フォントを表示するエリア
		 * @param [in]テキストのサイズ
		 * @param [in]2文字以降テキストをずらす値
		 * @param [in]文字の太さ
		 * @param [in]フォント番号
		 */
		void SetText(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col);

		/**
		 * @brief テキスト設定(フォントサイズや間隔を自動調整)
		 * @param [in]font
		 * @param [in]一文字目のpos
		 * @param [in]フォントを表示するエリア
		 * @param [in]テキストのサイズ
		 * @param [in]2文字以降テキストをずらす値
		 * @param [in]文字の太さ
		 * @param [in]フォント番号
		 */
		void SetTextAdjustFontSize(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col, bool isLinebrake);

		void UpdatePos(D3DXVECTOR3 first_pos);

		/**
		 * @brief 削除
		 * @param [in]font
		 */
		void Release();

		//リストの取得
		std::vector<CFont*> GetList() { return m_Font; }

		void SetAlign(TextAlign align) { m_align = align; }

		int GetTextShift() { return m_base_text_shift; }

	private:

		void TextShift(D3DXVECTOR3& text_pos, D3DXVECTOR2& text_shift, D3DXVECTOR3& save_first_pos);

		//メンバ変数
		std::vector<CFont*> m_Font;	//敵を管理

		const wchar_t* m_text;
		bool m_isLineBreak;  //改行するかどうか
		int m_base_text_shift; //文字をずらす値
		int m_thickness; //文字の太さ
		float m_base_size; //文字の大きさ
		D3DXVECTOR2 m_font_area;	//フォントのエリア
		TextAlign m_align; //どこに揃えるか
	};
}

#endif