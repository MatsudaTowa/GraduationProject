//=============================================
//
//�L�����N�^�[����[character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "character.h"
#include "colision.h"
#include "player.h"
#include "field.h"

//=============================================
//�R���X�g���N�^
//=============================================
My::CCharacter::CCharacter(int nPriority) :CObjectX(nPriority),
m_PartsCnt(INT_ZERO),											//�p�[�c��
m_pMotionFrameCnt(nullptr),										//���[�V�����̃t���[����
m_nKeySetCnt(INT_ZERO),											//�L�[�̌�
m_Motion(INT_ZERO),												//���[�V�������
m_isLoopFinish(),												//���[�v���I�������
m_motion_data(),												//���[�V�����ݒ�
m_pShadow(),													//�e�̃|�C���^
m_ShadowSize(VEC3_RESET_ZERO)									//�e�̃T�C�Y
{//�C�j�V�����C�U�[�Ńv���C�I���e�B�ݒ�A�e�����o�ϐ�������
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CCharacter::~CCharacter()
{
}

//=============================================
//������
//=============================================
HRESULT My::CCharacter::Init()
{
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ GetPos().x,SHADOW_POS_Y,GetPos().z }, m_ShadowSize);

		//�e�̃T�C�Y�ݒ�
		m_pShadow->SetSize({ m_ShadowSize });
	}

	if (m_pMotionFrameCnt == nullptr)
	{
		m_pMotionFrameCnt = new CCount;
		m_pMotionFrameCnt->SetCnt(INT_ZERO);
	}
	//�ŏ��ǂ̃��[�V�����ł��Ȃ��l����
	m_Motion = -1;
	//���[�v���[�V�����I����Ă锻���
	m_isLoopFinish = true;

	//�e�N���X�̏�����
	CObjectX::Init();

	//���[���h�}�g���b�N�X�ݒ�
	ConversionMtxWorld();
	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CCharacter::Uninit()
{
	if (m_pMotionFrameCnt != nullptr)
	{
		delete m_pMotionFrameCnt;
		m_pMotionFrameCnt = nullptr;
	}
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
	for (int i = INT_ZERO; i < MAX_PARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			m_apModel[i]->Unload();
		}
	}

	CObjectX::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CCharacter::Update()
{
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetisDraw(GetisDraw());

		//�e�̃T�C�Y�ݒ�
		m_pShadow->SetSize(m_ShadowSize);
		//�e�̈ʒu�ݒ�
		m_pShadow->SetPos({ GetPos().x,SHADOW_POS_Y,GetPos().z });
	}

	for (int nCnt = 0; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->SetOldPos({ m_apModel[nCnt]->GetMtxWorld()._41,m_apModel[nCnt]->GetMtxWorld()._42,m_apModel[nCnt]->GetMtxWorld()._43 });
	}

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	//���W���X�V
	SetPos(pos);
}

//=============================================
//�`��
//=============================================
void My::CCharacter::Draw()
{
	CObjectX::Draw();
}

//=============================================
//���[�V�����p�̕`��
//=============================================
void My::CCharacter::MotionDraw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX MtxWorld = GetMtxWorld();

	//�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorld);

	//���e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	SetMtxWorld(MtxWorld);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);


	for (int nCnt = INT_ZERO; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//=============================================
//�p�[�c�̃��[�h
//=============================================
void My::CCharacter::Load_Parts(const char* FileName)
{
	//�t�@�C���̓ǂݍ���
	std::ifstream File(FileName, std::ios::binary);

	//�t�@�C�����J���Ȃ�������֐��𔲂���
	if (!File.is_open())
	{
		return;
	}

	File.read(reinterpret_cast<char*>(&m_motion_data), sizeof(CCharacter::MotionData));

	m_PartsCnt = m_motion_data.parts;

	for (int nCnt = 0; nCnt < m_motion_data.parts; ++nCnt)
	{
		if (m_apModel[nCnt] != nullptr) { continue; }
		m_apModel[nCnt] = CModel_Parts::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, &m_motion_data.path[nCnt][0]);
		m_apModel[nCnt]->SetIdx(m_motion_data.idx[nCnt]);
		m_apModel[nCnt]->SetIdxParent(m_motion_data.parent[nCnt]);

		//�e��ݒ�
		if (m_apModel[nCnt]->GetIdxParent() == -1)
		{
			m_apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			m_apModel[nCnt]->SetParent(m_apModel[m_apModel[nCnt]->GetIdxParent()]);
		}

		m_apModel[nCnt]->SetPos(m_motion_data.parts_pos[nCnt]);
		m_apModel[nCnt]->SetRot(m_motion_data.parts_rot[nCnt]);

		m_apModel[nCnt]->SetTPos(m_apModel[nCnt]->GetPos());
		m_apModel[nCnt]->SetTRot(m_apModel[nCnt]->GetRot());
	}

	File.close();
}

//=============================================
//���[�V��������
//=============================================
void My::CCharacter::Motion()
{
	D3DXVECTOR3 MovePos[MAX_PARTS];
	D3DXVECTOR3 MoveRot[MAX_PARTS];

	int nNextKey = (m_nKeySetCnt + INT_ONE) % m_motion_data.motion_set[m_Motion].nNumKey;

	for (int nMotionCnt = INT_ZERO; nMotionCnt < GetNumParts(); nMotionCnt++)
	{
		D3DXVECTOR3 current_pos = m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].key[nMotionCnt].pos;
		D3DXVECTOR3 next_pos = m_motion_data.motion_set[m_Motion].keySet[nNextKey].key[nMotionCnt].pos;

		D3DXVECTOR3 current_rot = m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].key[nMotionCnt].rot;
		D3DXVECTOR3 next_rot = m_motion_data.motion_set[m_Motion].keySet[nNextKey].key[nMotionCnt].rot;

		float frame = (float)m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].nFrame;

		//���݂Ƃ̍����v�Z
		MovePos[nMotionCnt] = (next_pos - current_pos) / frame;
		MoveRot[nMotionCnt] = (next_rot - current_rot) / frame;
		D3DXVECTOR3 pos = m_apModel[nMotionCnt]->GetPos();
		D3DXVECTOR3 rot = m_apModel[nMotionCnt]->GetRot();

		pos += MovePos[nMotionCnt];
		rot += MoveRot[nMotionCnt];

		m_apModel[nMotionCnt]->SetPos({ pos.x,m_apModel[nMotionCnt]->GetPos().y,pos.z });
		m_apModel[nMotionCnt]->SetRot(rot);
	}

	if (m_pMotionFrameCnt->CountMeasure() > m_motion_data.motion_set[m_Motion].keySet[m_nKeySetCnt].nFrame)
	{

		m_pMotionFrameCnt->SetCnt(INT_ZERO);
		m_nKeySetCnt = (m_nKeySetCnt + INT_ONE) % m_motion_data.motion_set[m_Motion].nNumKey;
		if (m_nKeySetCnt == INT_ZERO)
		{
			if (m_motion_data.motion_set[m_Motion].nLoop == INT_ZERO)
			{
				//�I���������
				m_isLoopFinish = true;
				return;
			}
			for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
			{
				m_apModel[nCntParts]->SetRot(m_motion_data.motion_set[m_Motion].keySet[0].key[nCntParts].rot);
			}
		}
	}
}

//=====================================
//�����Ŏw�肵�����[�V�����ɐ؂�ւ���
//=====================================
void My::CCharacter::SetMotion(int Motion)
{
	if (m_Motion == Motion)
	{//���̃��[�V�����ƈ�v���Ă�����
		//���̊֐��𔲂���
		return;
	}

	m_Motion = Motion;

	//�t���[�����Z�b�g
	m_pMotionFrameCnt->SetCnt(INT_ZERO);

	//�L�[�J�E���g���Z�b�g
	m_nKeySetCnt = INT_ZERO;

	if (m_motion_data.motion_set[m_Motion].nLoop == INT_ZERO)
	{
		//�I���������
		m_isLoopFinish = false;
	}

	for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
	{
		m_apModel[nCntParts]->SetPos(m_apModel[nCntParts]->GetTPos());
		m_apModel[nCntParts]->SetRot(m_apModel[nCntParts]->GetTRot());

		m_apModel[nCntParts]->SetRot(m_motion_data.motion_set[Motion].keySet[0].key[nCntParts].rot);
	}
}

//=============================================
//���[���h�}�g���b�N�X�̕ϊ�
//=============================================
void My::CCharacter::ConversionMtxWorld()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX MtxWorld = GetMtxWorld();

	//�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);

	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

	for (int nCnt = INT_ZERO; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->ConversionMtxWorld();
	}
}