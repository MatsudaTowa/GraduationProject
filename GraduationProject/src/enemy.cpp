//=============================================
//
//�G�l�~�[[enemy.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy.h"
#include "game_manager.h"

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
	CCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //�p�[�c�ǂݍ���

	//�������[�V�����ݒ�
	SetMotion(MOTION_NEUTRAL);

	Motion(); //���[�V��������

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //�X�N���[�����W�ɕϊ�

	//if (GetLifeUI() == nullptr)
	//{
	//	CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
	//	SetLifeUI(pLifeUI);
	//}

	//if (GetEnergyUI() == nullptr)
	//{
	//	CEnergy_UI* pEnergyUI = CEnergy_UI::Create({screen_pos.x + 100.0f,screen_pos.y,screen_pos.z});
	//	SetEnergyUI(pEnergyUI);
	//}

	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CEnemy::Uninit()
{
	//�e�N���X�̏I���������Ă�
	CCharacter::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CEnemy::Update()
{
	CCharacter::Update();

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
My::CEnemy* My::CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) { return nullptr; }
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->Init();
	return pEnemy;
}
