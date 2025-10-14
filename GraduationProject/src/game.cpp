//=============================================
//
//�Q�[��[game.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game.h"
#include "game_field.h"
#include "game_player.h"
#include "game_camera.h"
#include "active_manager.h"
#include "enemy.h"
#include "card.h"

namespace
{
	const D3DXVECTOR3 FIELD_SIZE = { 200.0f,0.0f,200.0f };
	const int COOL_TIME = 1;
	const int DELAY_FRAME = 90;
}

//=============================================
//�R���X�g���N�^
//=============================================
My::CGame::CGame():
m_pPauseCnt(nullptr),				//�|�[�Y�̃L�[��L�������邽�߂̃J�E���g
m_pDelayCnt(nullptr)				//���U���g�ɑJ�ڂ���܂ł̃f�B���C�J�E���g
{
	CGameManager::GetInstance()->SetFinish(false);
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CGame::~CGame()
{
}

//=============================================
//������
//=============================================
HRESULT My::CGame::Init()
{
	CREATE_CAMERA(new CGameCamera);

	GET_CAMERA(0)->ChangeCameraState(new CBirdView);

	SET_CAMERA_IDX(0);

	CGameState* current_state = CGameManager::GetInstance()->GetState();
	if (current_state == nullptr)
	{
		current_state = new CNormal;
	}		
	CGameManager::GetInstance()->SetState(current_state);

	if (m_pPauseCnt == nullptr)
	{
		m_pPauseCnt = new CCount;
		m_pPauseCnt->SetFrame(COOL_TIME);
	}

	if (m_pDelayCnt == nullptr)
	{
		m_pDelayCnt = new CCount;
		m_pDelayCnt->SetFrame(DELAY_FRAME);
	}
	//�n�ʐ���
	CField::Create(VEC3_RESET_ZERO, FIELD_SIZE,new CGameField);

	//�v���C���[����
	CPlayer::Create(new CGamePlayer);

	CEnemy::Create({ 300.0f,0.0f,00.0f }, { 0.0f,1.75f,0.f });
	CEnemy::Create({ -300.0f,0.0f,00.0f }, { 0.0f,-1.75f,0.f });
	CEnemy::Create({ 0.0f,0.0f,250.0f }, { 0.0f,0.0f,0.f });

	//CCard::Create();

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CGame::Uninit()
{
	////�W�F�C�\���̏��폜
	//if (CJson::GetJson() != nullptr)
	//{
	//	delete CJson::GetJson();
	//	CJson::GetJson() = nullptr;
	//}

	if (m_pDelayCnt != nullptr)
	{
		delete m_pDelayCnt;
		m_pDelayCnt = nullptr;
	}

	if (m_pPauseCnt != nullptr)
	{
		delete m_pPauseCnt;
		m_pPauseCnt = nullptr;
	}

	//�X�e�[�g������
	CGameManager::GetInstance()->ChangeState(new CNormal);

	//�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�������ō폜
	//NOTE: �I�u�W�F�N�g��ReleaseAll�O�ɏ������Ƃœ�d�ō폜���邱�Ƃ�h�~
	CGameManager::GetInstance()->Uninit();

	CObject::ReleaseAll();
}

//=============================================
//�X�V
//=============================================
void My::CGame::Update()
{
	if (!CGameManager::GetInstance()->GetFinish())
	{
		//�|�[�Y�̃J�E���g�A�b�v
		m_pPauseCnt->CountUp();
//
//#ifdef _DEBUG
		//�C���v�b�g�擾
		CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
		}
//#endif // _DEBUG


		CGameManager::GetInstance()->GameStateExecution(this);
		return;
	}

	if (!m_pDelayCnt->CountUp())
	{
		return;
	}
	//�X�e�[�g������
	CGameManager::GetInstance()->ChangeState(new CNormal);
	GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
}

//=============================================
//�I�u�W�F�N�g�̍X�V���s��������
//=============================================
void My::CGame::StopObject(bool isStop)
{
	//�v���C�I���e�B�̐���������
	for (int i = 0; i < CObject::PRI_MAX; i++)
	{
		CObject* pObj = CObject::GetTopObject(i);	//�擪�擾

		//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();	//���̃|�C���^���擾

			if (pObj->GetStop() != isStop)
			{
				pObj->SetStop(isStop);
			}

			pObj = pNext;					//�|�C���^��i�߂�
		}
	}
}

//=============================================
//�`��
//=============================================
void My::CGame::Draw()
{
	GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
}
