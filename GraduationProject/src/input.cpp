//=============================================
//
//�C���v�b�g[input.cpp]
//Author Matsuda Towa
//
//=============================================
#include "input.h"
#include "manager.h"

//�ÓI�����o������
LPDIRECTINPUT8 My::CInput::m_pInput = nullptr;

/**
 * @brief �R���X�g���N�^
 */
My::CInput::CInput():
m_pDevice(nullptr)	//�f�o�C�X������
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CInput::~CInput()
{
}

/**
 * @brief ������
 * @param [in]�n���h���C���X�^���X
 * @param [in]�n���h�������h
 * @return �����������ǂ���
 */
HRESULT My::CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CInput::Uninit()
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//������L�[�{�[�h

/**
 * @brief �R���X�g���N�^
 */
My::CInputKeyboard::CInputKeyboard()
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CInputKeyboard::~CInputKeyboard()
{
}


/**
 * @brief ������
 * @param [in]�n���h���C���X�^���X
 * @param [in]�n���h�������h
 * @return �����������ǂ���
 */
HRESULT My::CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	My::CInput::Init(hInstance,hWnd);
	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�̃A�N�Z�X�����l��
	m_pDevice->Acquire();
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CInputKeyboard::Uninit()
{
	My::CInput::Uninit();
}

/**
 * @brief �X�V
 */
void My::CInputKeyboard::Update()
{
	BYTE aKeyState[NUM_KEY_MAX]; //���͏��
	int nCntKey;
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = INT_ZERO; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey]; //�����[�X
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire(); //�L�[�{�[�h�̃A�N�Z�X�����l��
	}
}
//������}�E�X

/**
 * @brief �R���X�g���N�^
 */
My::CInputMouse::CInputMouse()
{
#ifdef _DEBUG
	m_faster_move = VEC3_RESET_ZERO;
#endif // _DEBUG

}

/**
 * @brief �f�X�g���N�^
 */
My::CInputMouse::~CInputMouse()
{
}

/**
 * @brief ������
 * @param [in]�n���h���C���X�^���X
 * @param [in]�n���h�������h
 * @return �����������ǂ���
 */
HRESULT My::CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	m_MousePos = VEC3_RESET_ZERO;
	m_MouseMove = VEC3_RESET_ZERO;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = INT_ZERO;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;


	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	//�}�E�X�̃A�N�Z�X�����l��
	m_pDevice->Acquire();
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CInputMouse::Uninit()
{
	CInput::Uninit();
}

/**
 * @brief �X�V
 */
void My::CInputMouse::Update()
{

	DIMOUSESTATE zdiMouseState;
	int nCntMouse;
	
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(zdiMouseState), &zdiMouseState)))
	{
		for (nCntMouse = INT_ZERO; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			m_KeyStateRelease.rgbButtons[nCntMouse] = (m_KeyState.rgbButtons[nCntMouse] ^ zdiMouseState.rgbButtons[nCntMouse]) & ~zdiMouseState.rgbButtons[nCntMouse];
			m_KeyStateTrigger.rgbButtons[nCntMouse] = (m_KeyState.rgbButtons[nCntMouse] ^ zdiMouseState.rgbButtons[nCntMouse]) & zdiMouseState.rgbButtons[nCntMouse];
			m_KeyState.rgbButtons[nCntMouse] = zdiMouseState.rgbButtons[nCntMouse]; //�L�[�{�[�h�̃v���X����ۑ�
		}
		
		m_MouseMove.x = (float)zdiMouseState.lX;
		m_MouseMove.y = (float)zdiMouseState.lY;

#ifdef _DEBUG
		float x = m_MouseMove.x;
		float y = m_MouseMove.y;

		std::abs(x);
		std::abs(y);
		if (m_faster_move.x < x)
		{
			m_faster_move.x = x;
		}
		if (m_faster_move.y < y)
		{
			m_faster_move.y = y;
		}
#endif // _DEBUG



		UpdateAngle();
	}
	else
	{
		m_pDevice->Acquire(); //�L�[�{�[�h�̃A�N�Z�X�����l��
	}

}

/**
 * @brief �����X�V
 */
void My::CInputMouse::UpdateAngle()
{
	float x = m_MouseMove.x;
	float y = m_MouseMove.y;

	// �����𒲂ׂ�
	if (y > 0)
	{
		//�������̓t���O��������
		for (int i = 0; i < MAX; ++i)
		{
			m_isAngle[i] = false;
		}

		if (x > 0)
		{ // �E��
			m_isAngle[RIGHT] = true;
			m_isAngle[DOWN] = true;
		}
		else if (x < 0)
		{ // ����
			m_isAngle[LEFT] = true;
			m_isAngle[DOWN] = true;
		}
		else
		{ // ��
			m_isAngle[DOWN] = true;
		}
	}
	else if (y < 0)
	{
		//�������̓t���O��������
		for (int i = 0; i < MAX; ++i)
		{
			m_isAngle[i] = false;
		}
		if (x > 0)
		{ // �E��
			m_isAngle[RIGHT] = true;
			m_isAngle[UP] = true;
		}
		else if (x < 0)
		{ // ����
			m_isAngle[LEFT] = true;
			m_isAngle[UP] = true;
		}
		else
		{ // ��
			m_isAngle[UP] = true;

		}
	}
	else
	{
		if (x > 0)
		{ // �E
			m_isAngle[RIGHT] = true;
		}
		else if (x < 0)
		{ // ��
			m_isAngle[LEFT] = true;
		}
	}
}

/**
 * @brief �}�E�X�̃f�o�b�O�\��
 */
void My::CInputMouse::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	POINT pMouseMove;
	GetCursorPos(&pMouseMove);
	sprintf(&aStr[0], "[mouse]\nmove:%.1f,%.1f,%.1f\n�ő��l:%.1f,%.1f\n\npos:%d,%d"
		, m_MouseMove.x, m_MouseMove.y, m_MouseMove.z,m_faster_move.x,m_faster_move.y ,pMouseMove.x, pMouseMove.y);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));

	DebugAngle();
#endif // _DEBUG
}

/**
 * @brief �}�E�X�̃f�o�b�O�\��
 */
void My::CInputMouse::DebugAngle()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n��:%d �E:%d ��:%d ��:%d"
		,m_isAngle[UP],m_isAngle[RIGHT],m_isAngle[LEFT],m_isAngle[DOWN]);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}


//������pad

/**
 * @brief �R���X�g���N�^
 */
My::CInputPad::CInputPad():m_Connect(false)
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CInputPad::~CInputPad()
{
}

/**
 * @brief ������
 * @param [in]�n���h���C���X�^���X
 * @param [in]�n���h�������h
 * @return �����������ǂ���
 */
HRESULT My::CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_Connect = false;

	XINPUT_STATE joykeyState; //�W���C�p�b�h�̓��͏��

	//�������̃N���A
	memset(&m_joyKeyState, INT_ZERO, sizeof(XINPUT_STATE));

	//�������̃N���A
	memset(&m_JoypadMotor, INT_ZERO, sizeof(XINPUT_VIBRATION));

	//XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(INT_ZERO, &joykeyState) == ERROR_SUCCESS)
	{
		m_Connect = true;
	}
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CInputPad::Uninit()
{
	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

/**
 * @brief �X�V
 */
void My::CInputPad::Update()
{
	XINPUT_STATE joykeyState; //�W���C�p�b�h�̓��͏��

		//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(INT_ZERO, &joykeyState) == ERROR_SUCCESS)
	{
		//�ڑ�����Ă��邩�ǂ���
		m_Connect = true;

		//�X�e�B�b�N�̍X�V
		UpdateStick(joykeyState);

		joykeyState.Gamepad.wButtons |= GetJoypadStick(joykeyState.Gamepad.sThumbLX, joykeyState.Gamepad.sThumbLY, JOYPAD_ZONE);

		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joyKeyState;
		m_joyKeyStateTrigger = Button & ~OldButton;    // �g���K�[����
		m_ajoyKeyStateRelease = ~Button & OldButton;    // �����[�X����

		// ���݂̎��Ԃ��擾����
		m_aJoypadCurrentTime = timeGetTime();

		if (joykeyState.Gamepad.wButtons && ((m_aJoypadCurrentTime - m_aJoypadExecLastTime) > JOYPAD_SPEED))
		{
			// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
			m_aJoypadExecLastTime = m_aJoypadCurrentTime;

			// �L�[�{�[�h�̃��s�[�g����ۑ�
			m_joyKeyStateRepeat = joykeyState.Gamepad.wButtons;
		}
		else
		{
			// �L�[�{�[�h�̃��s�[�g����ۑ�
			m_joyKeyStateRepeat = INT_ZERO;
		}

		m_joyKeyState = joykeyState.Gamepad.wButtons;                // �v���X����
	}
	else
	{
		m_Connect = false;
	}
}

/**
 * @brief �X�e�B�b�N�̍X�V
 * @param [in]X�C���v�b�g�̏��
 */
void My::CInputPad::UpdateStick(XINPUT_STATE state)
{
	for (int nCntStick = 0; nCntStick < STICKTYPE_MAX; nCntStick++)
	{
		float fX,fY = 0.0f; //�X�e�B�b�N��X���AY��

		switch (nCntStick)
		{
		case STICKTYPE_LEFT:
			fX = (state.Gamepad.sThumbLX);
			fY = (state.Gamepad.sThumbLY);
			break;
		case STICKTYPE_RIGHT:
			fX = (state.Gamepad.sThumbRX);
			fY = (state.Gamepad.sThumbRY);
			break;
		}

		//�p�x���擾
		m_stick.afAngle[nCntStick] = FindAngle(D3DXVECTOR3(fX, fY, 0.0f),VEC3_RESET_ZERO) * -1;

		//�X�e�B�b�N�̓|������擾
		m_stick.afTplDiameter[nCntStick] = fabsf(fX);

		if (m_stick.afTplDiameter[nCntStick] < fabsf(fY))
		{
			m_stick.afTplDiameter[nCntStick] = fabsf(fY);
		}

		m_stick.afTplDiameter[nCntStick] /= 32768.0f; //�|���Ă����Ԃ̏����l

		//�������̓t���O��������
		for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
		{
			m_bAngle[nCntStick][nCntAngle] = false;
		}

		if (m_stick.afTplDiameter[nCntStick] > 0.1f)
		{
			//�p�x���l�����ŏ�Ɉʒu���鎞�A��t���O��^�ɂ���
			if ((m_stick.afAngle[nCntStick] < D3DX_PI * -0.75) || (m_stick.afAngle[nCntStick] > D3DX_PI * 0.75))
			{
				m_bAngle[nCntStick][STICKANGLE_UP] = true;
			}

			//�p�x���l�����ŉ��Ɉʒu���鎞�A���t���O��^�ɂ���
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.25))
			{
				m_bAngle[nCntStick][STICKANGLE_DOWN] = true;
			}

			//�p�x���l�����ō��Ɉʒu���鎞�A���t���O��^�ɂ���
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.75) && (m_stick.afAngle[nCntStick] < D3DX_PI * -0.25))
			{
				m_bAngle[nCntStick][STICKANGLE_LEFT] = true;
			}

			//�p�x���l�����ŉE�Ɉʒu���鎞�A�E�t���O��^�ɂ���
			else if ((m_stick.afAngle[nCntStick] > D3DX_PI * 0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.75))
			{
				m_bAngle[nCntStick][STICKANGLE_RIGHT] = true;
			}
		}

		//�p�x�ɉ��������͏���
		for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
		{
			//�X�e�B�b�N�̃g���K�[����ۑ�
			m_stick.abAngleTrigger[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & m_bAngle[nCntStick][nCntAngle];

			//�X�e�B�b�N�̃����[�X����ۑ�
			m_stick.abAngleRelease[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & ~m_bAngle[nCntStick][nCntAngle];

			//���݂̎��Ԃ��擾
			m_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

			if (m_bAngle[nCntStick][nCntAngle] && ((m_aStickCurrentTime[nCntStick][nCntAngle] - m_aStickExecLastTime[nCntStick][nCntAngle]) > JOYPAD_SPEED))
			{
				//�Ō�ɐ^��Ԃ������Ԃ�ۑ�
				m_aStickExecLastTime[nCntStick][nCntAngle] = m_aStickCurrentTime[nCntStick][nCntAngle];

				//�X�e�B�b�N�̃��s�[�g����ۑ�
				m_stick.abAngleRepeat[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
			}
			else
			{
				//�X�e�B�b�N�̃��s�[�g����ۑ�
				m_stick.abAngleRepeat[nCntStick][nCntAngle] = 0;
			}

			//�X�e�B�b�N�̃v���X����ۑ�
			m_stick.abAnglePress[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
		}
	}
}

/**
 * @brief �X�e�B�b�N�̎擾
 */
My::CInputPad::STICKINPUT My::CInputPad::GetStick(void)
{
	return m_stick;
}

/**
 * @brief �p�x�v�Z
 * @param [in]�ʒu
 * @param [in]�ړI�̈ʒu
 */
float My::CInputPad::FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos)
{
	float fAngle; //�p�x

	fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1.0f;

	return fAngle;
}

/**
 * @brief �X�e�B�b�N�̎擾
 * @param [in]X�̓|����Ă���l
 * @param [in]Y�̓|����Ă���l
 * @param [in]�l����
 */
WORD My::CInputPad::GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}
