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

	//�G���A����
	CreateArea();

	//�v���C���[����
	CPlayer::Create(new CGamePlayer);

	CEnemy::Create({ 300.0f,0.0f,00.0f }, { 0.0f,1.75f,0.f });
	CEnemy::Create({ -300.0f,0.0f,00.0f }, { 0.0f,-1.75f,0.f });
	CEnemy::Create({ 0.0f,0.0f,250.0f }, { 0.0f,0.0f,0.f });

	CCard::Create();

	return S_OK;
}

//=============================================
//�G���A�̐���
//=============================================
void My::CGame::CreateArea()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		CArea* area = CGameManager::GetInstance()->GetArea(i);
		if (area != nullptr) { continue; }

		//�O�p�`�̒��_���W���w��
		D3DXVECTOR2 triangle_vtx[CObject2D_Triangle::NUM_VERTEX];

		//�E�B���h�E�̒��S�ɂ�K�����_��
		D3DXVECTOR2 center = { SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF };
		triangle_vtx[1] = center;

		//���ꂼ��̒��_�ʒu���w��
		switch (i)
		{
		case CInputMouse::AREA::UP:
			triangle_vtx[0] = { SCREEN_WIDTH,FLOAT_ZERO };
			triangle_vtx[2] = { VEC2_RESET_ZERO };
			break;
		case CInputMouse::AREA::DOWN:
			triangle_vtx[0] = { FLOAT_ZERO,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::LEFT:
			triangle_vtx[0] = { FLOAT_ZERO,FLOAT_ZERO };
			triangle_vtx[2] = { FLOAT_ZERO,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::RIGHT:
			triangle_vtx[0] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,FLOAT_ZERO };
			break;
		default:
			break;
		}
		area = CArea::Create(triangle_vtx);

		CGameManager::GetInstance()->SetArea(area, i);
	}
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
	CGameManager::GetInstance()->Uninit();

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
		//TODO:�Q�[���X�e�[�g�ŃJ�[�h�������Ă���Ƃ��ɂ��̊֐������s
		SelectArea();

		//�|�[�Y�̃J�E���g�A�b�v
		m_pPauseCnt->CountUp();

#ifdef _DEBUG
		//�C���v�b�g�擾
		CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		if (pKeyboard->GetTrigger(DIK_RETURN))
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
//�G���A�̑I��
//=============================================
void My::CGame::SelectArea()
{
	CInputMouse::AREA area = GET_INPUT_MOUSE->GetArea();

	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		CGameManager::GetInstance()->GetArea(i)->SetSelect(false);
		if (area == CInputMouse::CENTER) { continue; }

		//�I������Ă���Ƃ���͖��邭
		CGameManager::GetInstance()->GetArea(area)->SetSelect(true);
	}
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
