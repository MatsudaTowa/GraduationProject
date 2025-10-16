//============================================
//
// 入力[input.cpp]
// Author:Uedakou
// 
//============================================

#ifndef _INPUT_H_
#define _INPUT_H_


namespace My {
	class CInput	// 入力
	{
	public:
		CInput();
		~CInput();
		virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
		virtual void Uninit();
		virtual void Update() = 0;;
	protected:
		static LPDIRECTINPUT8 m_pInput;
		LPDIRECTINPUTDEVICE8 m_pDevice;
	private:
	};

	// キーボード入力
#define NUM_KEY_MAX (256)
#define ON_VALUE (0x80)		// オン状態の値
	class CInputKeyboard : public CInput
	{
	public:
		CInputKeyboard();
		~CInputKeyboard();
		HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
		void Uninit()override;
		void Update()override;

		bool GetPless();	// 全キープレス取得
		bool GetTrigger();	// 全キープレス取得
		bool GetRepeat();	// 全キープレス取得
		bool GetRelease();	// 全キープレス取得
		bool GetPless(int Key) { return (m_aPless[Key] & 0x80) != 0; }		// プレス取得
		bool GetTrigger(int Key) { return (m_aTrigger[Key] & 0x80) != 0; }	// トリガー取得
		bool GetRepeat(int Key) { return (m_aRepeat[Key] & 0x80) != 0; }	// リピート取得
		bool GetRelease(int Key) { return (m_aRelease[Key] & 0x80) != 0; }	// リリース取得
	private:
		BYTE m_aPless[NUM_KEY_MAX];		// プレス
		BYTE m_aTrigger[NUM_KEY_MAX];	// トリガー
		BYTE m_aRepeat[NUM_KEY_MAX];	// リピート
		BYTE m_aRelease[NUM_KEY_MAX];	// リリース
	};

	// マウス入力
	class CInputMouse : public CInput
	{
	public:
		typedef enum
		{
			MOUSEBUTTON_LEFT = 0,	// 左クリック
			MOUSEBUTTON_RIGHT,		// 右クリック
			MOUSEBUTTON_CENTER,		// 中クリック
			MOUSEBUTTON_BACK,		// 戻る
			MOUSEBUTTON_MAX
		}MouseButton;	// マウスキー種類
		CInputMouse();
		~CInputMouse();
		HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
		void Uninit()override;
		void Update()override;

		bool GetPress(MouseButton button_type);   	// プレス入力
		bool GetTrigger(MouseButton button_type);	// トリガー入力
		bool GetRepeat(MouseButton button_type);	// リピート入力
		bool GetRelease(MouseButton button_type);	// リリース入力

		D3DXVECTOR2 GetPos();
		D3DXVECTOR2 GetMove();
		int GetMouseHweel();
	private:
		DIMOUSESTATE m_PrevMouseState;			//!< マウスの１フレーム前の入力情報
		DIMOUSESTATE m_CurrentMouseState;		//!< マウスの現在の入力情報
		D3DXVECTOR2 m_MousePos;					// マウス位置
		D3DXVECTOR2 m_MouseMove;				// マウス動き
		long m_lHweel;
	};

	// ジョイパッド

#define MAX_CONTROLLERS 4 // XInputが認識できるコントローラーの最大数

	class CInputJoypad
	{
	public:
		// ジョイキーの種類
		typedef enum
		{
			JOYKEY_UP = 0,	// 十字キー（上）
			JOYKEY_DOWN,	// 十字キー（下）
			JOYKEY_LEFT,	// 十字キー（左）
			JOYKEY_RIGHT,	// 十字キー（右）
			JOYKEY_START,	// スタート
			JOYKEY_BACK,	// バック
			JOYKEY_L3,		// L3
			JOYKEY_R3,		// R3
			JOYKEY_LB,		// L1
			JOYKEY_RB,		// R1
			JOYKEY_LT,		// L2	// 反応なし
			JOYKEY_RT,		// R2	// 反応なし
			JOYKEY_A,		// A
			JOYKEY_B,		// B
			JOYKEY_X,		// Transform
			JOYKEY_Y,		// Y
			JOYKEY_MAX
		}JOYKEY;	// ジョイパッドキー入力種類
		typedef enum
		{
			JOYSTICK_LEFT = 0, // 左スティック
			JOYSTICK_RIGHT,	// 右スティック
			JOYSTICK_MAX
		}JOYSTICK;	// ジョイパッドスティック入力種類

		typedef enum
		{
			STICKANGLE_UP = 0, // 上
			STICKANGLE_DOWN,   // 下
			STICKANGLE_LEFT,   // 左
			STICKANGLE_RIGHT,  // 右
			STICKANGLE_MAX,
		}STICKANGLE;

		typedef struct
		{
			float afSlope;							// スティックの角度
			float afRot;							// スティックの向き
			bool abAnglePress[STICKANGLE_MAX];		// スティックの方向プレス情報
			bool abAngleTrigger[STICKANGLE_MAX];	// スティックの方向トリガー情報
			bool abAngleRepeat[STICKANGLE_MAX];		// スティックの方向リピート情報
			bool abAngleRelease[STICKANGLE_MAX];	// スティックの方向リリース情報

		}STICK_STATE;
		CInputJoypad();
		~CInputJoypad();
		// ジョイキー
		HRESULT Init();						// ジョイパッド初期化
		void Uninit();						// ジョイパッド終了
		void Update();						// ジョイパッド更新
		// ジョイキー
		bool GetJoykyePress() {
			for (int nCnt = 0; nCnt < JOYKEY_MAX; nCnt++)
			{
				if ((m_joiKeyState.Gamepad.wButtons & (0x01 << nCnt)) != 0)
				{
					return true;
				}
			}
			return false;
		}
		bool GetJoykyePress(JOYKEY kye);	// ジョイパッド(プレス)
		bool GetJoykyeTrigger(JOYKEY kye);	// ジョイパッド(トリガー)
		bool GetJoykyeRepeat(JOYKEY kye);	// ジョイパッド(リピート)
		bool GetJoykyeRelease(JOYKEY kye);	// ジョイパッド(リリース)

		// ジョイスティック
		float GetJoystickSlope(JOYSTICK stick);	// 角度取得
		float GetJoystickRot(JOYSTICK stick);	// 向き取得

		bool GetJoystickPress(JOYSTICK stick, STICKANGLE angle);	// ジョイパッド(プレス)
		bool GetJoystickTrigger(JOYSTICK stick, STICKANGLE angle);	// ジョイパッド(トリガー)
		bool GetJoystickRepeat(JOYSTICK stick, STICKANGLE angle);	// ジョイパッド(リピート)
		bool GetJoystickRelease(JOYSTICK stick, STICKANGLE angle);	// ジョイパッド(リリース)

	private:
		::XINPUT_STATE m_joiKeyState;			// ジョイパッドプレス情報
		::XINPUT_STATE m_joiKeyStateTrigger;	// ジョイパッドプレス情報
		::XINPUT_STATE m_joiKeyStateRepeat;	// ジョイパッドプレス情報
		::XINPUT_STATE m_joiKeyStateRelease;	// ジョイパッドプレス情報

		STICK_STATE m_stickState[JOYSTICK_MAX];											//スティックの入力情報
	};

}

#endif // !_INPUT_H_