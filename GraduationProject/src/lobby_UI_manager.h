//=============================================
//
//ロビー全体のUI管理「lobby_UI_manager.h」
// Author松田永久
//
//=============================================
#ifndef _LOBBY_UI_MANAGER_H_ //これが定義されてないとき
#define _LOBBY_UI_MANAGER_H_
#include "main.h"
#include "active_scene_character.h"
namespace My
{
	class CReadyUI;
	//=============================================
	//ロビー全体のUIクラス
	//=============================================
	class CLobbyUIManager
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CLobbyUIManager();
		/**
		 * @brief デストラクタ
		 */
		~CLobbyUIManager();
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

		inline CReadyUI* GetReadyUI()
		{
			return m_pReadyUI;
		}

		inline void SetReadyUI(CReadyUI* readyUI)
		{
			m_pReadyUI = readyUI;
		}

	private:
		CReadyUI* m_pReadyUI;
	};
}


#endif