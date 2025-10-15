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

//=============================================
//�R���X�g���N�^
//=============================================
My::CPlayer::CPlayer(int nPriority) :CActiveSceneCharacter(nPriority)
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
	CActiveSceneCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //�p�[�c�ǂݍ���

	//�������[�V�����ݒ�
	SetMotion(MOTION_NEUTRAL);

	//�e�̃T�C�Y�ݒ�
	SetShadowSize({SHADOW_SIZE_X,0.0f,SHADOW_SIZE_Z});
	Motion(); //���[�V��������

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CPlayer::Uninit()
{
	//�e�N���X�̏I���������Ă�
	CActiveSceneCharacter::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CPlayer::Update()
{
	CActiveSceneCharacter::Update();
	
	Motion(); //���[�V��������
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
My::CPlayer* My::CPlayer::Create(CPlayer* player, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int id)
{
	CModel* pModel = GET_MODEL;

	CPlayer* pPlayer = player;

	// null�Ȃ�null��Ԃ�
	if (pPlayer == nullptr) { return nullptr; }

	pPlayer->Init(); //����������
	pPlayer->SetPos(pos);
	pPlayer->SetRot(rot);
	pPlayer->SetPlayerIdx(id);

	pPlayer->SetType(OBJECT_TYPE_PLAYER); //�^�C�v�ݒ�

	return pPlayer;
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

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z);
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
