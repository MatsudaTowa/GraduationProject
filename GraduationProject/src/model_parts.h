//=============================================
// 
//�e�q�֌W�̃p�[�c[model.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MODEL_PARTS_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _MODEL_PARTS_H_
#include "main.h"
#include "objectX.h"

/** @brief My ���O��� */
namespace My
{

	/** @brief ���f���p�[�c�N���X */
	class CModel_Parts
	{
	public:
		static const int MAX_MODEL = 256; //!<���f���̐�

		/** @brief ���f�����\���� */
		struct MODEL_INFO
		{
			LPD3DXMESH s_pMesh;			//!<���b�V�����
			LPD3DXBUFFER s_pBuffMat;	//!<�}�e���A�����
			DWORD s_dwNumMat;			//!<�}�e���A����;
			char* s_ModelName;			//!<���f���l�[���ۑ��p
		};

		/**
		 * @brief �R���X�g���N�^
		 */
		CModel_Parts();

		/**
		 * @brief �f�X�g���N�^
		 */
		virtual ~CModel_Parts();

		/**
		 * @brief ���f���j��
		 */
		virtual void Unload();

		/**
		 * @brief X�t�@�C���ݒ�
		 * @param [in]�}�e���A���o�b�t�@
		 * @param [in]�}�e���A����
		 * @param [in]���b�V�����
		 */
		virtual void BindXFile(LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPD3DXMESH pMesh);

		/**
		 * @brief �`��
		 */
		virtual void Draw();

		/**
		 * @brief �`��
		 * @param [in]�J���[
		 */
		virtual void Draw(D3DXCOLOR col);

		/**
		 * @brief ���[���h�}�g���b�N�X�v�Z
		 */
		virtual void ConversionMtxWorld();

		/**
		 * @brief �p�[�c����
		 * @param [in]�ʒu
		 * @param [in]����
		 * @param [in]���f���̃t�@�C���p�X
		 * @return ���f���̃|�C���^
		 */
		static CModel_Parts* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char* pModel_Path);

		/**
		 * @brief ���[���h�}�g���b�N�X�ݒ�
		 * @param [in]���[���h�}�g���b�N�X
		 */
		void SetMtxWorld(D3DXMATRIX mtxWorld)
		{
			m_mtxWorld = mtxWorld;
		}

		/**
		 * @brief �ʒu�ݒ�
		 * @param [in]�ʒu
		 */
		void SetPos(D3DXVECTOR3 pos)
		{
			m_pos = pos;
		};

		/**
		 * @brief �ߋ��̈ʒu�ݒ�
		 * @param [in]�ߋ��̈ʒu
		 */
		void SetOldPos(D3DXVECTOR3 oldpos)
		{
			m_oldpos = oldpos;
		};

		/**
		 * @brief �����ݒ�
		 * @param [in]����
		 */
		void SetRot(D3DXVECTOR3 rot)
		{
			m_rot = rot;
		};

		/**
		 * @brief �ԍ��ݒ�
		 * @param [in]�ԍ�
		 */
		void SetIdx(int idx)
		{
			m_nIdx = idx;
		}

		/**
		 * @brief �e�̐ݒ�
		 * @param [in]�e�̔ԍ�
		 */
		void SetIdxParent(int parent)
		{
			m_nIdxModelParent = parent;
		}

		/**
		 * @brief T���̈ʒu
		 * @param [in]�ʒu���W
		 */
		void SetTPos(D3DXVECTOR3 Tpos)
		{
			m_Tpos = Tpos;
		}

		/**
		 * @brief T���̕���
		 * @param [in]����
		 */
		void SetTRot(D3DXVECTOR3 Trot)
		{
			m_Trot = Trot;
		}

		/**
		 * @brief �ʒu�擾
		 * @return �ʒu
		 */
		D3DXVECTOR3 GetPos()
		{
			return m_pos;
		};

		/**
		 * @brief �ߋ��̈ʒu�擾
		 * @return �ߋ��̈ʒu
		 */
		D3DXVECTOR3 GetOldPos()
		{
			return m_oldpos;
		};

		/**
		 * @brief �����擾
		 * @return ����
		 */
		D3DXVECTOR3 GetRot()
		{
			return m_rot;
		};

		/**
		 * @brief T�|�[�Y�ʒu�擾
		 * @return T�|�[�Y�ʒu
		 */
		D3DXVECTOR3 GetTPos()
		{
			return m_Tpos;
		};

		/**
		 * @brief T�|�[�Y�����擾
		 * @return T�|�[�Y����
		 */
		D3DXVECTOR3 GetTRot()
		{
			return m_Trot;
		};

		/**
		 * @brief �ԍ�
		 * @return ���f���ԍ�
		 */
		int GetIdx()
		{
			return m_nIdx;
		}

		/**
		 * @brief �e�̎擾
		 * @return �e�̔ԍ�
		 */
		int GetIdxParent()
		{
			return m_nIdxModelParent;
		}

		/**
		 * @brief �ŏ��l�擾
		 * @return �ŏ��l
		 */
		D3DXVECTOR3 GetMin()
		{
			return m_minpos;
		}

		/**
		 * @brief �ő�l�擾
		 * @return �ő�l
		 */
		D3DXVECTOR3 GetMax()
		{
			return m_maxpos;
		}

		/**
		 * @brief �e�̐ݒ�
		 * @param [in]�e�̃��f���p�[�c
		 */
		void SetParent(CModel_Parts* pParent);

		/**
		 * @brief ���[���h�}�g���b�N�X�擾
		 * @return ���[���h�}�g���b�N�X
		 */		
		D3DXMATRIX& GetMtxWorld();

		/**
		 * @brief ���f�����擾
		 * @param [in]���f���̔ԍ�
		 * @return ���f�����
		 */
		MODEL_INFO GetModelInfo(int nIdx);

	private:
		LPDIRECT3DTEXTURE9 m_pTexture[256]; //!<�e�N�X�`���ւ̃|�C���^
		D3DXMATRIX m_mtxWorld;				//!<���[���h�}�g���b�N�X
		MODEL_INFO m_ModelInfo[MAX_MODEL];	//!<���f�����

		D3DXVECTOR3 m_pos;					//!<�ʒu
		D3DXVECTOR3 m_rot;					//!<����
		D3DXVECTOR3 m_Tpos;					//!<T�|�[�Y�̈ʒu
		D3DXVECTOR3 m_Trot;					//!<T�|�[�Y�̕���
		int m_nIdx;							//!<���Ԗڂ̃p�[�c��
		int m_nIdxModelParent;				//!<�e�̃C���f�b�N�X

		D3DXVECTOR3 m_minpos;				//!<pos�ŏ��l
		D3DXVECTOR3 m_maxpos;				//!<pos�ő�l
		D3DXVECTOR3 m_oldpos;				//!<�ߋ��̈ʒu
		int m_nNumAll;						//!<���f������

		CModel_Parts* m_pParent;			//!<�e�p�[�c�ւ̃|�C���^
	};
}

#endif