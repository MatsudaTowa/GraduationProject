//===========================================
// 
// ���C��[main.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _MAIN_H_	// ���̃}�N����`����ĂȂ��Ȃ�
#define _MAIN_H_	// ��d�C���N���[�h�h�~�̃}�N��

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")		// �`��ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9,lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	// �V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	// D�C���v�b�g
#pragma comment(lib,"xinput.lib")	// X�C���v�b�g



#define IS_DELAY_UPDATE (false)	// �x���t���X�V�����邩�ǂ��������ݒ�

namespace My
{

	class CApplication
	{
	public:
		CApplication();
		~CApplication();

		bool Init(HINSTANCE hInstance, int nCmdShow); // ����������
		void Uninit();	// �������
		void Run();		// ���C�����[�v
		void OnKeyDown(WPARAM key);	// �L�[�{�[�h���͂ɑ΂��鏈��

	private:
		HWND m_hWnd;				// �A�v���P�[�V�����̃E�B���h�E�n���h��
		WNDCLASSEX m_wcex;			// �E�B���h�E�N���X���\����
		MSG m_msg;					// ���b�Z�[�W�\����
		DWORD m_dwExecLastTime;		// �Ō�ɏ������s�������ԁims�j
		DWORD m_dwFPSLostTime;		// �Ō��FPS���v���������ԁims�j
		DWORD m_dwFrameCount;		// �t���[�����J�E���g�iFPS�v���p�j
		bool m_siDelayUpdate;		// �x���t���X�V�����邩�ǂ���
	};
	int GetFPS();	// FPS�擾
}
#endif // _MAIN_H_