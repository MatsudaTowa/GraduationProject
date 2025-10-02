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
m_bLanding(false),												//���n���Ă��邩
m_move(VEC3_RESET_ZERO),										//�ړ���
m_nLife(INT_ZERO),												//�̗�
m_oldpos(VEC3_RESET_ZERO),										//�ߋ��̈ʒu
m_PartsCnt(INT_ZERO),											//�p�[�c��
m_nMotionFrameCnt(INT_ZERO),									//���[�V�����̃t���[����
m_nKeySetCnt(INT_ZERO),											//�L�[�̌�
m_Motion(INT_ZERO),												//���[�V�������
m_isLoopFinish(),												//���[�v���I�������
m_Speed(),														//�X�s�[�h
m_Jump(),														//�W�����v��
m_motion_data(),													//���[�V�����ݒ�
m_nJumpCnt(INT_ZERO),											//�W�����v��
m_pShadow(),													//�e�̃|�C���^
m_ShadowSize(VEC3_RESET_ZERO)									//�e�̃T�C�Y
{//�C�j�V�����C�U�[�Ńv���C�I���e�B�ݒ�A�e�����o�ϐ�������
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CCharacter::~CCharacter()
{
	for (int i = INT_ZERO; i < MAX_PARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			m_apModel[i]->Unload();
		}
	}
}

//=============================================
//������
//=============================================
HRESULT My::CCharacter::Init()
{
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ GetPos().x,SHADOW_POS_Y,GetPos().z }, m_ShadowSize);
	}

	//�e�̃T�C�Y�ݒ�
	m_pShadow->SetSize({ m_ShadowSize });

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
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	CObjectX::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CCharacter::Update()
{
	m_pShadow->SetisDraw(GetisDraw());

	for (int nCnt = 0; nCnt < m_PartsCnt; nCnt++)
	{
		m_apModel[nCnt]->SetOldPos({ m_apModel[nCnt]->GetMtxWorld()._41,m_apModel[nCnt]->GetMtxWorld()._42,m_apModel[nCnt]->GetMtxWorld()._43 });
	}

	//�d�͏���
	Gravity();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	if (m_bLanding)
	{
		//�ړ��ʂ��X�V(�����j
		m_move *= FLOAT_ONE - MOVE_FRICTION;
	}

	//�ߋ��̈ʒu�ɍ��̈ʒu����
	m_oldpos = pos;

	//�ړ��ʒǉ�
	pos += m_move;
	//���W���X�V
	SetPos(pos);

	//�e�̃T�C�Y�ݒ�
	m_pShadow->SetSize(m_ShadowSize);

	//�e�̈ʒu�ݒ�
	m_pShadow->SetPos({ GetPos().x,SHADOW_POS_Y,GetPos().z });

	//�ő�ŏ��l�擾
	D3DXVECTOR3 minpos = GetMinPos();
	D3DXVECTOR3 maxpos = GetMaxPos();

	if (m_bLanding)
	{//���n���Ă�Ȃ�
		//�W�����v�����Z�b�g
		m_nJumpCnt = INT_ZERO;
	}

	//���Ƃ̐ڐG����
	HitField();
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

	m_Speed = m_motion_data.s_speed;
	m_Jump = m_motion_data.s_jump;
	m_PartsCnt = m_motion_data.s_parts;

	for (int nCnt = 0; nCnt < m_motion_data.s_parts; ++nCnt)
	{
		m_apModel[nCnt] = CModel_Parts::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, &m_motion_data.s_path[nCnt][0]);
		m_apModel[nCnt]->SetIdx(m_motion_data.s_idx[nCnt]);
		m_apModel[nCnt]->SetIdxParent(m_motion_data.s_parent[nCnt]);

		//�e��ݒ�
		if (m_apModel[nCnt]->GetIdxParent() == -1)
		{
			m_apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			m_apModel[nCnt]->SetParent(m_apModel[m_apModel[nCnt]->GetIdxParent()]);
		}

		m_apModel[nCnt]->SetPos(m_motion_data.s_parts_pos[nCnt]);
		m_apModel[nCnt]->SetRot(m_motion_data.s_parts_rot[nCnt]);

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

	int nNextKey = (m_nKeySetCnt + INT_ONE) % m_motion_data.s_motion_set[m_Motion].s_nNumKey;

	for (int nMotionCnt = INT_ZERO; nMotionCnt < GetNumParts(); nMotionCnt++)
	{
		MovePos[nMotionCnt] = (m_motion_data.s_motion_set[m_Motion].s_keySet[nNextKey].s_key[nMotionCnt].s_pos - m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_key[nMotionCnt].s_pos) / (float)m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame;
		MoveRot[nMotionCnt] = (m_motion_data.s_motion_set[m_Motion].s_keySet[nNextKey].s_key[nMotionCnt].s_rot - m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_key[nMotionCnt].s_rot) / (float)m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame;

		D3DXVECTOR3 pos = m_apModel[nMotionCnt]->GetPos();
		D3DXVECTOR3 rot = m_apModel[nMotionCnt]->GetRot();

		pos += MovePos[nMotionCnt];
		rot += MoveRot[nMotionCnt];

		m_apModel[nMotionCnt]->SetPos({ pos.x,m_apModel[nMotionCnt]->GetPos().y,pos.z });
		m_apModel[nMotionCnt]->SetRot(rot);
	}

	m_nMotionFrameCnt++;

	if (m_nMotionFrameCnt > m_motion_data.s_motion_set[m_Motion].s_keySet[m_nKeySetCnt].s_nFrame)
	{

		m_nMotionFrameCnt = INT_ZERO;
		m_nKeySetCnt = (m_nKeySetCnt + INT_ONE) % m_motion_data.s_motion_set[m_Motion].s_nNumKey;
		if (m_nKeySetCnt == INT_ZERO)
		{
			if (m_motion_data.s_motion_set[m_Motion].s_nLoop == INT_ZERO)
			{
				//�I���������
				m_isLoopFinish = true;
				return;
			}
			for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
			{
				m_apModel[nCntParts]->SetRot(m_motion_data.s_motion_set[m_Motion].s_keySet[0].s_key[nCntParts].s_rot);
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
	m_nMotionFrameCnt = INT_ZERO;

	//�L�[�J�E���g���Z�b�g
	m_nKeySetCnt = INT_ZERO;

	if (m_motion_data.s_motion_set[m_Motion].s_nLoop == INT_ZERO)
	{
		//�I���������
		m_isLoopFinish = false;
	}

	for (int nCntParts = INT_ZERO; nCntParts < m_PartsCnt; nCntParts++)
	{
		m_apModel[nCntParts]->SetPos(m_apModel[nCntParts]->GetTPos());
		m_apModel[nCntParts]->SetRot(m_apModel[nCntParts]->GetTRot());

		m_apModel[nCntParts]->SetRot(m_motion_data.s_motion_set[Motion].s_keySet[0].s_key[nCntParts].s_rot);
	}
}


//=============================================
//�d�͏���
//=============================================
void My::CCharacter::Gravity()
{
	if (m_move.y < GRAVITY_MAX)
	{
		m_move.y -= GRAVITY_MOVE;
	}
}

//=============================================
//�W�����v����
//=============================================
void My::CCharacter::Jump()
{
	m_move.y = m_Jump; //�W�����v�͑��
	m_bLanding = false; //�󒆏��
}

//=============================================
//���Ƃ̐ڐG����
//=============================================
void My::CCharacter::HitField()
{
	D3DXVECTOR3 CharacterPos = GetPos();

	//�T�C�Y�擾
	D3DXVECTOR3 CharacterMin = GetMinPos();
	D3DXVECTOR3 CharacterMax = GetMaxPos();
	//�G�Ƃ̓����蔻��
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = CObject::GetTopObject(i);	//�擪�擾

		//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();	//���̃|�C���^���擾

			//�G�������đ��x���グ��
			if (pObj->GetType() == CObject::OBJECT_TYPE_FIELD)
			{
				CField* pField = dynamic_cast<CField*>(pObj);

				//���Ƃ̓����蔻��
				CColision::COLISION colision = CManager::GetInstance()->GetColision()->CheckColision_Y(m_oldpos, CharacterPos, CharacterMin, CharacterMax, pField->GetPos(), pField->GetSize());

				if (colision == CColision::COLISION::COLISON_TOP_Y)
				{//y(��)�����ɓ������Ă���
					CharacterPos.y = m_oldpos.y;
					m_move.y = FLOAT_ZERO;
					m_bLanding = true; //���n
				}
				else
				{
					m_bLanding = false; //���n
				}

				if (m_oldpos.x > pField->GetPos().x - pField->GetSize().x
					&& CharacterPos.x <= pField->GetPos().x - pField->GetSize().x)
				{
					CharacterPos.x = m_oldpos.x;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.x < pField->GetPos().x + pField->GetSize().x
					&& CharacterPos.x >= pField->GetPos().x + pField->GetSize().x)
				{
					CharacterPos.x = m_oldpos.x;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.z > pField->GetPos().z - pField->GetSize().z
					&& CharacterPos.z <= pField->GetPos().z - pField->GetSize().z)
				{
					CharacterPos.z = m_oldpos.z;
					m_move.x = FLOAT_ZERO;
				}

				if (m_oldpos.z < pField->GetPos().z + pField->GetSize().z
					&& CharacterPos.z >= pField->GetPos().z + pField->GetSize().z)
				{
					CharacterPos.z = m_oldpos.z;
					m_move.x = FLOAT_ZERO;
				}
				SetPos(CharacterPos);

			}
			pObj = pNext;							//�|�C���^��i�߂�

		}
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