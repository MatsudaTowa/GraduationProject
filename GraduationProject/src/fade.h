//=============================================
//
//フェード処理[fade.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _FADE_H_ //これが定義されてないとき

#define _FADE_H_
#include "main.h"
#include "scene.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{

	/**
	 * @brief フェードクラス
	 * @copydoc CObject2D
	 */		
	class CFade : public CObject2D
	{
	public:

		/** @brief フェードの状態 */
		enum FADE
		{
			FADE_NONE = 0,	//!<何もしていない状態
			FADE_IN,		//!<フェードイン
			FADE_OUT,		//!<フェードアウト
			FADE_MAX,		//!<フェードのマックス
		};

		static constexpr int FADE_PRIORITY = 29;  //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */	
		CFade(int nPriority = FADE_PRIORITY);

		/**
		 * @brief デストラクタ
		 */	
		~CFade()override;

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
		 * @brief フェード設定
		 * @param [in]次のシーン
		 */	
		void SetFade(CScene::MODE modeNext);

		/**
		 * @brief フェードの色取得
		 * @return 現在のフェードの色
		 */			
		float FadeColor(void);

		/**
		 * @brief フェードの生成
		 * @param [in]生成位置
		 * @return フェードのポインタ
		 */		
		static CFade* Create(D3DXVECTOR3 pos);
	private:
		static constexpr float FADE_VALUE = 0.05f;  //!<アルファ値減衰量
		FADE m_fade;								//!<フェード状態
		CScene::MODE m_next_scene;					//!<次のシーン
		float m_colorFade;							//!<フェードの色
	};
}

#endif