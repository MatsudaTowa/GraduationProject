//***************************************************************************************************
//
// imgui(manager.h)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef _MANAGER_H_
#define _MANAGER_H_

// include
#include "main.h"
#include "renderer.h"
#include "input.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My マネージャークラス */
	class CManager
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		CManager();

		/**
		* @brief デストラクタ
		*/
		~CManager();

		/**
		* @brief 初期化処理
		* @param [in]インスタンス
		* @param [in]ウィンドウハンドル
		* @param [in]フラグ
		* @return 成功したかどうか
		*/
		HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

		/**
		* @brief 終了処理
		*/
		void Uninit();

		/**
		* @brief 更新処理
		*/
		void Update();

		/**
		* @brief 描画処理
		*/
		void Draw();

		static CManager* GetInstance()
		{
			static CManager instance; // 静的インスタンス
			return &instance;
		}

		/**
		* @brief レンダラー取得処理
		* @return レンダラーのポインタ
		*/
		CRenderer* GetRenderer();

		/**
		* @brief キーボード取得処理
		* @return キーボードのポインタ
		*/
		CInputKeyboard* GetKeyboard();

		/**
		* @brief マウス取得処理
		* @return マウスのポインタ
		*/
		CInputMouse* GetMouse();

	private:
		CRenderer* m_pRenderer;				// レンダラーのポインタ
		CInputKeyboard* m_pInputKeyboard;	// キーボードのポインタ
		CInputMouse* m_pInputMouse;				// マウスのポインタ
	};
}
#endif