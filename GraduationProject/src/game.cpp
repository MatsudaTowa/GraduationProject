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
#include "game_manager.h"

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
	for (int i = 0; i < CAMERA_MAX; ++i)
	{
		CREATE_CAMERA(new CGameCamera);
		switch (i)
		{
		case GAME_CAMERA:
			GET_CAMERA(i)->ChangeCameraState(new CBirdView);
			break;
		case EVENT_CAMERA_000:
			GET_CAMERA(i)->ChangeCameraState(new CBirdView);
			break;
		case EVENT_CAMERA_001:
			GET_CAMERA(i)->ChangeCameraState(new CBirdView);
			break;
		case BOARD_GENERATE_CAMERA:
			GET_CAMERA(i)->ChangeCameraState(new CBirdView);
			break;
		default:
			break;
		}
	}
	SET_CAMERA_IDX(GAME_CAMERA);

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
	CGameManager::GetInstance()->GetPlayer()->Uninit();
	CGameManager::GetInstance()->SetPlayer(nullptr);
	CGameManager::GetInstance()->GetField()->Uninit();
	CGameManager::GetInstance()->SetField(nullptr);

	CObject::ReleaseAll();
	CGameManager::GetInstance()->Uninit();
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

		GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
		GET_CAMERA(BOARD_GENERATE_CAMERA)->SetCamera();
		GET_CAMERA(BOARD_GENERATE_CAMERA)->ChangeCameraState(new CBirdView);

#ifdef _DEBUG
		//�C���v�b�g�擾
		CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		if (pKeyboard->GetTrigger(DIK_RETURN) || pMouse->GetTrigger(0))
		{
			GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
		}
#endif // _DEBUG


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
	GET_CAMERA(BOARD_GENERATE_CAMERA)->SetCamera();
}
