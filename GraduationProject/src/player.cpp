//=============================================
//
//�v���C���[[player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "player.h"
#include "manager.h"
#include "input.h"
#include "field.h"
#include "camera.h"
#include "camera_state.h"

//�X�|�[���ʒu
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_POS = { 0.0f, 0.0f, 0.0f };

//�X�|�[������
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_ROT = { 0.0f, 0.0f, 0.0f};

//�e�̃T�C�Y
const D3DXVECTOR3 My::CPlayer::SHADOW_SIZE = { 20.0f, 0.0, 20.0f };

//�ʏ�̈ړ���R
const float My::CPlayer::DAMPING_COEFFICIENT = 0.3f;

//�x���g�̕␳�l
const float My::CPlayer::CORRECTION_VALUE_BELT = -0.5f;

//=============================================
//�R���X�g���N�^
//=============================================
My::CPlayer::CPlayer(int nPriority) :CCharacter(nPriority)
{//�C�j�V�����C�U�[�Ń����o�ϐ�������
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CPlayer::~CPlayer()
{
}

//=============================================
//������
//=============================================
HRESULT My::CPlayer::Init()
{
	CCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //�p�[�c�ǂݍ���

	//�X�|�[�����̐ݒ�
	SetPos(CPlayer::PLAYER_SPAWN_POS);
	SetRot(CPlayer::PLAYER_SPAWN_ROT);
	SetLife(CPlayer::PLAYER_LIFE);

	//�ړ��ʏ�����
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���[�u�l���
	SetMove(move);

	//�������[�V�����ݒ�
	SetMotion(MOTION_NEUTRAL);

	//�e�̃T�C�Y�ݒ�
	SetShadowSize(SHADOW_SIZE);
	Motion(); //���[�V��������

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CPlayer::Uninit()
{
	//�e�N���X�̏I���������Ă�
	CCharacter::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CPlayer::Update()
{
	CCharacter::Update();
	
	Motion(); //���[�V��������

	if (GetPos().y > KILL_Y)
	{
		return;
	}

	ReSpawn();
}

//=============================================
//�`��
//=============================================
void My::CPlayer::Draw()
{
	//�e�N���X�̃��[�V�����p�̕`����Ă�
	MotionDraw();

	//�v���C���[�̃f�o�b�O�\��
	Debug();
}

//=============================================
//����
//=============================================
My::CPlayer* My::CPlayer::Create(CPlayer* player)
{
	CModel* pModel = GET_MODEL;

	CPlayer* pPlayer = player;

	// null�Ȃ�null��Ԃ�
	if (pPlayer == nullptr) { return nullptr; }

	pPlayer->Init(); //����������

	pPlayer->SetType(OBJECT_TYPE_PLAYER); //�^�C�v�ݒ�

	return pPlayer;
}

//=============================================
//���X�|�[��
//=============================================
void My::CPlayer::ReSpawn()
{
	//�������g��pos�擾
	D3DXVECTOR3 PlayerPos = GetPos();

	//�X�|�[�����̐ݒ�ɂ��ǂ�
	SetPos(CPlayer::PLAYER_SPAWN_POS);
	SetMove({0.0f,0.0f,0.0f});
	SetRot(CPlayer::PLAYER_SPAWN_ROT);
	SetLife(CPlayer::PLAYER_LIFE);
}

//=============================================
//�ړ�����
//=============================================
void My::CPlayer::Input()
{
}

//=============================================
//�v���C���[�̕������J�����̂ق���
//=============================================
void My::CPlayer::ResetRot()
{
	float rotMoveY = CManager::GetInstance()->GetCamera(0)->GetRot().y;

	//�e�N���X����rot���擾
	D3DXVECTOR3 rot = GetRot();

	rot.y = rotMoveY + D3DX_PI;

	SetRot(rot);
}

//=============================================
//�v���C���[�̃f�o�b�O�\��
//=============================================
void My::CPlayer::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = CManager::GetInstance()->GetRenderer()->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\nmove:%.1f,%.1f,%.1f"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetMove().x, GetMove().y, GetMove().z);
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
