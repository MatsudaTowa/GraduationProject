//=============================================
//
//準備できてるかのUI「ready_UI.h」
// Author松田永久
//
//=============================================
#ifndef _READY_UI_H_ //これが定義されてないとき
#define _READY_UI_H_
#include "main.h"
#include "object2D.h"
#include "number.h"
#include "active_scene_character.h"

#include "ready_txt.h"
namespace My
{
	//=============================================
	//準備できているかのUIクラス
	//=============================================
	class CReadyUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CReadyUI();
		/**
		 * @brief デストラクタ
		 */
		~CReadyUI();
		/**
		 * @brief 初期化
		 * @param [in]キャラクターのポインタ
		 * @return 成功したか
		 */
		HRESULT Init(CActiveSceneCharacter* character);
		/**
		 * @brief 終了
		 */
		void Uninit();
		/**
		 * @brief 更新
		 */
		void Update();
		/**
		 * @brief 現在のスモークUI設定
		 * @param [in]キャラクターのポインタ
		 */
		void SetCurrentReady_UI(CActiveSceneCharacter* character);

		/**
		 * @brief リセット
		 */
		void Reset();

		inline CReadyTxt* GetReadyTxt()
		{
			return m_pReady_txt;
		}

		inline void SetReadyTxt(CReadyTxt* txt)
		{
			m_pReady_txt = txt;
		}

	private:
		CReadyTxt* m_pReady_txt;
	};
}


#endif