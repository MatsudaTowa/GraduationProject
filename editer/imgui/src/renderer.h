//***************************************************************************************************
//
// 2Dゲーム基盤(renderer.h)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef _RENDERER_H_
#define _RENDERER_H_

// include
#include "main.h"
#include "edit.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My レンダラークラス */
	class CRenderer
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CRenderer();

		/**
		* @brief デストラクタ
		*/
		~CRenderer();

		/**
		* @brief 初期化処理
		* @param [in]ウィンドウハンドル
		* @param [in]フラグ
		* @return 成功したかどうか
		*/
		HRESULT Init(HWND hWnd, BOOL bWindow);

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

		/**
		* @brief デバイスのポインタ取得
		* @return デバイスのポインタ
		*/
		LPDIRECT3DDEVICE9 GetDevice();			

	private:
		LPDIRECT3D9 m_pD3D;				// オブジェクトのポインタ
		LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイスのポインタ
	
		CEdit* m_pEdit;					// エディターのポインタ

		int m_ID;
		int m_Step;
	};

}
#endif 

