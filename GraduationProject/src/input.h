//=============================================
//
//入力デバイス[input.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _INPUT_H_ //定義されてなかったら
#define _INPUT_H_ //２種インクルード防止
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief インプットクラス */
	class CInput
	{
	public:
		CInput();
		~CInput();
		virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
		virtual void Uninit();
		virtual void Update() = 0;//端末ごとに
	protected:
		static LPDIRECTINPUT8 m_pInput;
		LPDIRECTINPUTDEVICE8 m_pDevice;
	};

	/** @brief キーボードクラス */
	class CInputKeyboard :public CInput
	{
	public:
		static constexpr int NUM_KEY_MAX = 256; //キーボードのキー数
		CInputKeyboard();
		~CInputKeyboard();
		HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
		void Uninit() override;
		void Update() override;//端末ごとに

		//プレス情報取得
		inline bool GetPress(int nKey)
		{
			return(m_aKeyState[nKey] & 0x80) != 0;
		}

		//トリガー情報取得
		inline bool GetTrigger(int nKey)
		{
			return(m_aKeyStateTrigger[nKey] & 0x80) != 0;
		}

		//リリース情報取得
		inline bool GetRelease(int nKey)
		{
			return(m_aKeyStateRelease[nKey] & 0x80) != 0;
		}
	private:
		BYTE m_aKeyStateTrigger[NUM_KEY_MAX]; //キーボードのトリガー情報
		BYTE m_aKeyStateRelease[NUM_KEY_MAX]; //キーボードのリリース情報
		BYTE m_aKeyState[NUM_KEY_MAX]; //キーボードのプレス情報
	};

	/** @brief マウスクラス */
	class CInputMouse :public CInput
	{
	public:
		static constexpr int NUM_MOUSE_MAX = 3; //マウスのキー数
		enum AREA
		{
			UP = 0,
			DOWN,
			LEFT,
			RIGHT,
			CENTER,
			MAX,
		};
		CInputMouse();
		~CInputMouse();
		HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
		void Uninit() override;
		void Update() override;

		//移動量設定
		inline void SetMouseMove(D3DXVECTOR3 move)
		{
			m_MouseMove = move;
		}

		//プレス情報取得
		inline bool GetPress(int nKey)
		{
			return(m_KeyState.rgbButtons[nKey] & 0x80) != 0;
		}

		//トリガー情報取得
		inline bool GetTrigger(int nKey)
		{
			return(m_KeyStateTrigger.rgbButtons[nKey] & 0x80) != 0;
		}

		//リリース情報取得
		inline bool GetRelease(int nKey)
		{
			return(m_KeyStateRelease.rgbButtons[nKey] & 0x80) != 0;
		}

		//位置取得
		inline D3DXVECTOR3 GetMousePos(void)
		{
			return m_MousePos;
		}

		//移動量取得
		inline D3DXVECTOR3 GetMouseMove(void)
		{
			return m_MouseMove;
		}

		//エリア取得
		inline AREA GetArea(void)
		{
			return m_area;
		}

		void Debug();
	private:
		static constexpr float CENTER_RADIUS = 15.0f;

		void DebugAngle();
		void SetMouseArea();

		DIMOUSESTATE m_KeyStateTrigger; //マウスのトリガー情報
		DIMOUSESTATE m_KeyStateRelease; //マウスのリリース情報
		DIMOUSESTATE m_KeyState; //マウスのプレス情報
		DIMOUSESTATE m_zdiMouseMove; //マウスムーブ
		D3DXVECTOR3 m_MousePos;
		D3DXVECTOR3 m_MouseMove;
		AREA m_area; //ウィンドウを四分割してエリアを判断する変数
#ifdef _DEBUG
		D3DXVECTOR3 m_faster_move; //最速のスピード(デバッグ用)
#endif // _DEBUG

	};

	/** @brief パッドクラス */
	class CInputPad :public CInput
	{
	public:
		//キーの種類
		enum JOYKEY
		{
			JOYKEY_UP = 0,
			JOYKEY_DOWN,
			JOYKEY_LEFT,
			JOYKEY_RIGHT,
			JOYKEY_START,
			JOYKEY_BACK,
			JOYKEY_L3,
			JOYKEY_R3,
			JOYKEY_LB,
			JOYKEY_RB,
			JOYKEY_LT,
			JOYKEY_RT,
			JOYKEY_A,
			JOYKEY_B,
			JOYKEY_X,
			JOYKEY_Y,
			JOYKEY_MAX
		};

		//スティックの番号
		enum STICKTYPE
		{
			STICKTYPE_LEFT = 0, // 左スティック
			STICKTYPE_RIGHT, // 右スティック
			STICKTYPE_MAX
		};

		//スティックの方向
		enum STICKANGLE
		{
			STICKANGLE_UP = 0, // 上
			STICKANGLE_DOWN,   // 下
			STICKANGLE_LEFT,   // 左
			STICKANGLE_RIGHT,  // 右
			STICKANGLE_MAX,
		};

		//スティックの入力情報
		struct STICKINPUT
		{
			float afTplDiameter[STICKTYPE_MAX];                      // スティックの倒し具合
			float afAngle[STICKTYPE_MAX];                            // スティックの角度
			bool abAnglePress[STICKTYPE_MAX][STICKANGLE_MAX];        // スティックの方向プレス情報
			bool abAngleTrigger[STICKTYPE_MAX][STICKANGLE_MAX];      // スティックの方向トリガー情報
			bool abAngleRepeat[STICKTYPE_MAX][STICKANGLE_MAX];       // スティックの方向リピート情報
			bool abAngleRelease[STICKTYPE_MAX][STICKANGLE_MAX];      // スティックの方向リリース情報
		};

		//マクロ定義
		static constexpr int JOYPAD_ZONE = 1;
		static constexpr int JOYPAD_SPEED = 15;

		CInputPad();
		~CInputPad();
		HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
		void Uninit() override;
		void Update() override;//端末ごとに

		//接続されているかどうか
		inline bool GetConnet()
		{
			return m_Connect;
		}

		//プレス情報取得
		inline bool GetPress(JOYKEY Key)
		{
			return (m_joyKeyState & (0x01 << Key)) ? true : false;
		}

		//トリガー情報取得
		inline bool GetTrigger(JOYKEY Key)
		{
			return (m_joyKeyStateTrigger & (0x01 << Key)) ? true : false;
		}

		//リリース情報取得
		inline bool GetRelease(JOYKEY Key)
		{
			return(m_ajoyKeyStateRelease & (0x01 << Key)) ? true : false;
		}


		inline XINPUT_STATE* GetXInputState(void)
		{
			return &m_XInput;
		}

		void UpdateStick(XINPUT_STATE state);
		STICKINPUT GetStick(void);
		float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos);
		WORD GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone);
	private:
		//コントローラー
		bool m_Connect; //接続されてるかどうか
		WORD m_joyKeyState; //ジョイパッドのプレス情報
		WORD m_joyKeyStateTrigger; //ジョイパッドのトリガー情報
		WORD m_joyKeyStateRepeat; //ジョイパッドのリピート情報
		WORD m_ajoyKeyStateRelease; //コントローラーのリリース情報
		WORD m_aJoypadCurrentTime; //コントローラーの現在の時間
		WORD m_aJoypadExecLastTime; //コントローラーの最後に真を返した時間
		WORD m_JoypadInput; //コントローラーの入力情報
		D3DXVECTOR3 m_joyStickPos; //スティックの傾き
		bool m_bAngle[STICKTYPE_MAX][STICKANGLE_MAX]; //スティックの入力情報
		DWORD m_aStickCurrentTime[STICKTYPE_MAX][STICKANGLE_MAX]; //スティックの現在の時間
		DWORD m_aStickExecLastTime[STICKTYPE_MAX][STICKANGLE_MAX]; //スティックの現在の時間
		STICKINPUT m_stick; //スティックの入力情報
		XINPUT_STATE m_XInput; //入力情報
		XINPUT_VIBRATION m_JoypadMotor;
		int m_nCntkey;
	};
}

#endif // _INPUT_H_ //定義されてなかったら