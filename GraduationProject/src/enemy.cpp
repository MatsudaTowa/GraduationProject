//=============================================
//
//�G�l�~�[[enemy.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy.h"
#include "active_manager.h"
#include "enemy_state.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CEnemy::CEnemy(int nPriority)
{
	//�}�l�[�W���[�ɓo�^
	CGameManager::GetInstance()->GetEnemyManager()->Regist(this);
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CEnemy::~CEnemy()
{
	//�}�l�[�W���[����폜
	if (CGameManager::GetInstance()->GetEnemyManager() != nullptr)
	{
		CGameManager::GetInstance()->GetEnemyManager()->Erase(this);
	}
}

//=============================================
// ������
//=============================================
HRESULT My::CEnemy::Init()
{
	if (GetState() == nullptr)
	{
		ChangeState(new CEnemyLobbyState);
	}

	CActiveSceneCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //�p�[�c�ǂݍ���

	//�������[�V�����ݒ�
	SetMotion(MOTION_NEUTRAL);

	Motion(); //���[�V��������

	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CEnemy::Uninit()
{
	//�e�N���X�̏I���������Ă�
	CActiveSceneCharacter::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CEnemy::Update()
{
	CActiveSceneCharacter::Update();

	Motion(); //���[�V��������
}

//=============================================
// �`��
//=============================================
void My::CEnemy::Draw()
{
	//�e�N���X�̃��[�V�����p�̕`����Ă�
	MotionDraw();
}

//=============================================
// ����
//=============================================
My::CEnemy* My::CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int id)
{
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) { return nullptr; }
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetPlayerIdx(id);
	pEnemy->Init();
	return pEnemy;
}
