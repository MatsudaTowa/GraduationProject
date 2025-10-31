//***************************************************************************************************
//
// imgui(input.h)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef _INPUT_H_
#define _INPUT_H_

// include
#include "main.h"
#include "renderer.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My 入力基底クラス */
	class CInput
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CInput();

		/**
		* @brief デストラクタ
		*/
		~CInput();

		/**
		* @brief 初期化処理
		* @param [in]インスタンス
		* @param [in]ウィンドウハンドル
		* @return 成功したかどうか
		*/
		virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);

		/**
		* @brief 終了処理
		*/
		virtual void Uninit();

		/**
		* @brief 更新処理
		*/
		virtual void Update() = 0;

	protected:
		static LPDIRECTINPUT8 m_pInput;							// DirectInputオブジェクトへのポインタ
		LPDIRECTINPUTDEVICE8 m_pDevice;							// 入力デバイスのポインタ
		int m_nPressTime;										// キーやボタンを押し続けた時間
	};

	/** @brief My キーボードクラス */
	class CInputKeyboard : public CInput
	{
	public:
		static const int NUM_KEY = 256;	// キーの数

		/**
		* @brief コンストラクタ
		*/
		CInputKeyboard();

		/**
		* @brief デストラクタ
		*/
		~CInputKeyboard();

		/**
		* @brief 初期化処理
		* @param [in]インスタンス
		* @param [in]ウィンドウハンドル
		* @return 成功したかどうか
		*/
		HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;

		/**
		* @brief 終了処理
		*/
		void Uninit()override;

		/**
		* @brief 更新処理
		*/
		void Update()override;

		/**
		* @brief プレス情報取得
		*/
		bool GetPress(int nKey);								

		/**
		* @brief トリガー情報取得
		*/
		bool GetTrigger(int nKey);								
		
	private:
		BYTE m_aKeyState[NUM_KEY];
		BYTE m_aKeyStateTrigger[NUM_KEY];

	};

	/** @brief My マウスクラス */
	class CInputMouse : public CInput
	{
	public:
		static const int MAX_BUTTON = 8;

		/**
		* @brief コンストラクタ
		*/
		CInputMouse();

		/**
		* @brief デストラクタ
		*/
		~CInputMouse();

		/**
		* @brief 初期化処理
		* @param [in]インスタンス
		* @param [in]ウィンドウハンドル
		* @return 成功したかどうか
		*/
		HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;

		/**
		* @brief 終了処理
		*/
		void Uninit()override;

		/**
		* @brief 更新処理
		*/
		void Update()override;

		/**
		* @brief マウスのプレス取得
		* @param [in]マウス情報
		* @return 押されたかどうか
		*/
		bool GetPress(int button);

		/**
		* @brief マウスのトリガー取得
		* @param [in]マウス情報
		* @return 押されたかどうか
		*/bool GetTrigger(int button);

		/**
		* @brief マウスの移動量取得(X軸)
		* @return マウスの移動量
		*/
		LONG GetDeltaX();

		/**
		* @brief マウスの移動量取得(Y軸)
		* @return マウスの移動量
		*/
		LONG GetDeltaY();

		/**
		* @brief マウスの移動量取得(マウスホイール)
		* @return マウスの移動量
		*/
		LONG GetDeltaZ();

	private:
		DIMOUSESTATE m_MouseState;			
		DIMOUSESTATE m_MouseStateTrigger;
	};

}
#endif