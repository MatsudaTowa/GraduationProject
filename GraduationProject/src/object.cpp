//=============================================
//
//�I�u�W�F�N�g[object.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include  "manager.h"

//����������
int My::CObject::m_nNumAll = 0;

//�I�u�W�F�N�g�|�C���^������
My::CObject* My::CObject::m_pCur[My::CObject::PRI_MAX] = { nullptr };			//�Ō���̃|�C���^�̏�����
My::CObject* My::CObject::m_pTop[My::CObject::PRI_MAX] = { nullptr };			//�ŏ��̃|�C���^�̏�����
/**
 * @brief �R���X�g���N�^
 * @param [in]�v���C�I���e�B
 */
My::CObject::CObject(int nPriority):
	m_isDelete(false),
	m_isStop(false),
	m_isDraw(true)
{
	m_nPriority = nPriority; //�`��D��x�ݒ�
	CObject* pObj = this;				//���g�̃|�C���^���擾
	m_nNumAll++;						//�������J�E���g

	pObj->m_pPrev = m_pCur[nPriority];	//�����̑O�̃I�u�W�F�N�g�|�C���^���擾

	//���߂ẴI�u�W�F�N�g�Ȃ�ÓI�����o�ɑ��
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = pObj;	//�ŏ��̃I�u�W�F�N�g�̃|�C���^
		m_pCur[nPriority] = pObj;	//�Ō���̃I�u�W�F�N�g�̃|�C���^
		m_pCur[nPriority]->m_pNext = nullptr;	//���̃|�C���^��������
	}
	else
	{
		m_pCur[nPriority]->m_pNext = pObj;	//���݂̍Ō���Ɏ��̃|�C���^��ݒ�
	}

	pObj->m_pNext = nullptr;			//���̃|�C���^��������
	m_pCur[nPriority] = pObj;			//�Ō�����X�V
}

/**
 * @brief �f�X�g���N�^
 */
My::CObject::~CObject()
{
	m_nNumAll--;
}

/**
 * @brief �S�č폜
 */
void My::CObject::ReleaseAll()
{
	//�ی��Ƃ��Đ�ɏI�����������s
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = m_pTop[i];	//�擪�擾

		//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//���̃|�C���^���擾

			if (pObj->m_type != CObject::OBJECT_TYPE::OBJECT_TYPE_FADE)
			{
				pObj->Uninit();					//�I������
			}

			pObj = pNext;					//�|�C���^��i�߂�
		}
	}
}

/**
 * @brief �S�čX�V
 */
void My::CObject::UpdateAll()
{
	//�ϐ��̐錾
	CObject* pObj = nullptr;

	//�v���C�I���e�B�̐���������
	for (int i = 0; i < PRI_MAX; i++)
	{
		pObj = m_pTop[i];	//�擪�擾

		//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//���̃|�C���^���擾

			if (pObj->m_isStop)
			{
				continue;
			}

			pObj->Update();				//�X�V����
			
			pObj = pNext;					//�|�C���^��i�߂�
		}
	}

	//�v���C�I���e�B�̐���������
	for (int i = 0; i < PRI_MAX; i++)
	{
		pObj = m_pTop[i];	//�擪�擾

	//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//���̃|�C���^���擾

			//���S�t���O�������Ă�����Release
			if (pObj->m_isDelete)
			{
				pObj->Release();
			}

			pObj = pNext;	//�|�C���^��i�߂�
		}
	}
}

/**
 * @brief �S�ĕ`��
 */
void My::CObject::DrawAll()
{
	//�v���C�I���e�B�̐���������
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = m_pTop[i];	//�擪�擾

		//�ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//���̃|�C���^���擾
			pObj->Draw();					//�X�V����
			pObj = pNext;					//�|�C���^��i�߂�
		}
	}
}


My::CObject* My::CObject::GetTopObject(int priority)
{
	//�v���C�I���e�B�͈͓̔��Ȃ�Ԃ�
	if (priority >= 0 && priority < PRI_MAX)
	{
		return m_pTop[priority];
	}

	//�͈͊O�Ȃ�nullptr
	return nullptr;
}

/**
 * @brief �������g�̍폜
 */
void My::CObject::Release()
{
	//�ϐ��ɑ��
	int nPriority = m_nPriority;

	//null�`�F�b�N
	if (this != nullptr)
	{
		CObject* pObj = this;					//���g�̃|�C���^���擾

		if (pObj->m_pPrev != nullptr)
		{
			pObj->m_pPrev->m_pNext = pObj->m_pNext;	//���g�̑O����q��
		}

		if (pObj->m_pNext != nullptr)
		{
			pObj->m_pNext->m_pPrev = pObj->m_pPrev;	//���g�̑O����q��
		}

		//�Ō�������݂̃|�C���^�Ȃ�O�̃|�C���^�ɍX�V
		if (pObj == m_pCur[nPriority])
		{
			m_pCur[nPriority] = pObj->m_pPrev;	//�|�C���^�̕ύX
		}

		//�ŏ������݂̃|�C���^�Ȃ玟�̃|�C���^�ɍX�V
		if (pObj == m_pTop[nPriority])
		{
			m_pTop[nPriority] = pObj->m_pNext;	//�|�C���^�̕ύX
		}

		//���X�g����폜
		delete pObj;
		pObj = nullptr;
	}
}
