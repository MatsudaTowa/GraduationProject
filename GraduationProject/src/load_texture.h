//================================
//
//load_texture.cppに必要な宣言[load_texture.h]
//Author:島津湧人
// 
//================================

#ifndef _LOAD_TEXTURE_H_ //このマクロ定義がされていなかったら
#define _LOAD_TEXTURE_H_ //２重インクルード防止のマクロ定義

namespace My
{
	//ロードテクスチャクラス
	class CLoadTexture
	{
	public:

		//メンバ関数
		CLoadTexture();		//コンストラクタ
		~CLoadTexture();		//デストラクタ

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CLoadTexture* GetInstance()
		{
			//静的インスタンス
			static CLoadTexture instance;
			return &instance;
		}

		/**
		 * @brief 読み込み
		 */
		void Load();

		/**
		 * @brief フラグの取得
		 * @return フラグ
		 */
		inline bool GetIsEndLoad() { return m_isEndLoad; }
		
	private:

		//読み込むUIの種類
		enum LOAD_UI
		{
			BUTTON = 0,
			ILUSST0,
			ILUSST1,
			ILUSST2,
			ICON,
			OTHER,
			MAX
		};

		//変数
		bool m_isEndLoad;	//読み込みが終わっているか
		LOAD_UI m_NextLoad;	//次に読み込むテクスチャ
	};
}

#endif