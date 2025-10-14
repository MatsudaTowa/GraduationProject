//=============================================
//
//�L�����N�^�[[character.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CHARACTER_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _CHARACTER_H_
#include "main.h"
#include "objectX.h"
#include "model_parts.h"
#include "shadow.h"
#include "count.h"
#include "useful.h"
#include "life_UI.h"
#include "energy_UI.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �L�����N�^�[�N���X */
	class CCharacter : public CObjectX
	{
	public:
		static constexpr int MAX_KEY = 20;					//!<�L�[�ő吔
		static constexpr int CHARACTER_PRIORITY = 8;		//!<�L�����N�^�[�̃v���C�I���e�B
		static constexpr int MAX_MOTION = 100;				//!<���[�V�����̍ő吔
		static constexpr int MAX_PARTS = 64;				//!<�ő�p�[�c��

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CCharacter(int nPriority = CHARACTER_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CCharacter()override;

		/**
		 * @brief ������
		 * @return ����������
		 */		
		HRESULT Init()override;

		/**
		 * @brief �I��
		 */	
		void Uninit()override;

		/**
		 * @brief �X�V
		 */	
		void Update()override;

		/**
		 * @brief �`��
		 */	
		void Draw()override;

		/**
		 * @brief ���[�V�����̕`��
		 */	
		void MotionDraw();

		/**
		 * @brief �p�[�c�̃��[�h
		 * @param [in]���f���̃t�@�C���l�[��
		 */	
		void Load_Parts(const char* FileName);

		/**
		 * @brief ���[�V����
		 */			
		void Motion();

		/**
		 * @brief �����Ŏw�肵�����[�V�����ɐ؂�ւ���
		 * @param [in]���[�V�����̗񋓎w��i���o�[
		 */			
		void SetMotion(int Motion);

		/**
		 * @brief ���[���h�}�g���b�N�X�ɕϊ�
		 */		
		void ConversionMtxWorld();

		 /**
		  * @brief ���[�V�����̃t�B�j�b�V���ݒ�
		  * @param [in]���[�V�����̃��[�v�I��������
		  */
		inline void SetFinish(bool isFinish)
		{
			m_isLoopFinish = isFinish;
		}

		 /**
		  * @brief �e�̃T�C�Y�ݒ�ݒ�
		  * @param [in]�e�̃T�C�Y
		  */
		inline void SetShadowSize(D3DXVECTOR3 shadow_size)
		{
			m_ShadowSize = shadow_size;
		}

		/**
		 * @brief �e�̃T�C�Y�擾
		 * @return �e�̃T�C�Y
		 */			
		inline D3DXVECTOR3& GetShadowSize()
		{
			return m_ShadowSize;
		}

		/**
		 * @brief ���[�V�����̃��[�v���I����Ă��邩�ǂ���
		 * @return �I����Ă��邩�ǂ���
		 */		
		inline bool& GetFinish()
		{
			return m_isLoopFinish;
		}

		/**
		 * @brief �p�[�c���擾
		 * @return �p�[�c��
		 */	
		inline int& GetNumParts()
		{
			return m_PartsCnt;
		}

		/**
		 * @brief ���݂̃L�[�擾
		 * @return �L�[��
		 */
		inline int& GetKeyCnt()
		{
			return m_nKeySetCnt;
		}

		/**
		 * @brief �e�擾
		 * @return ���݂̃W�����v��
		 */
		inline CShadow* GetShadow()
		{
			return m_pShadow;
		}

		/**
		 * @brief �p�[�c�擾
		 * @param [in]�p�[�c�ԍ�
		 * @return ���f���p�[�c�̃|�C���^
		 */
		inline CModel_Parts* GetModelParts(int idx)
		{
			return m_apModel[idx];
		}
	private:
		static constexpr float SHADOW_POS_Y = 0.5f;	 //!<�e��Y���W(�n�ʂ��班��������)

		bool m_isLoopFinish;		//!<���[�v���[�V�������I�������
		int m_PartsCnt;				//!<�p�[�c��
		CCount* m_pMotionFrameCnt;	//!���[�V�����t���[���J�E���g
		int m_nKeySetCnt;			//!<�L�[�J�E���g
		int m_Motion;				//!<���[�V����(�e�I�u�W�F�N�g����񋓂��󂯎��)
		D3DXCOLOR m_col;			//!<�J���[
		D3DXVECTOR3 m_ShadowSize;	//!<�e�̃T�C�Y

		CShadow* m_pShadow;			//!<�e

		CModel_Parts* m_apModel[MAX_PARTS]; 		//!<�p�[�c

		/**
		 * @brief �L�[�\����
		 */	
		struct Key
		{
			D3DXVECTOR3 pos;		//!<�ʒu
			D3DXVECTOR3 rot;		//!<����
			D3DXVECTOR3 Trot;		//!<T�|�[�Y���̕���
		};

		/**
		 * @brief �L�[�ݒ�\����
		 */	
		struct KeySet
		{
			int nFrame;			//!<�t���[����
			Key key[MAX_KEY];		//!<�L�[���
		};

		/**
		 * @brief ���[�V�����ݒ�\����
		 */	
		struct MotionSet
		{
			int nLoop;					//!<���[�v���邩�ǂ���
			int nNumKey;					//!<�L�[��
			KeySet keySet[MAX_PARTS];		//!<�L�[�ݒ���
		};

		/**
		 * @brief ���[�V�����ǂݍ��ݏ��\����
		 */
		struct MotionData
		{
			MotionSet motion_set[MAX_MOTION];		//!<���[�V�����ݒ�
			int parts;							//!<�p�[�c��
			int idx[MAX_PARTS];					//!<�p�[�c�ԍ�
			int parent[MAX_PARTS];				//!<�e�̔ԍ�
			float speed;							//!<���x
			float jump;							//!<�W�����v��
			char path[MAX_PARTS][256];		//!<�t�@�C���p�X
			D3DXVECTOR3 parts_pos[MAX_PARTS];		//!<�p�[�c�̈ʒu
			D3DXVECTOR3 parts_rot[MAX_PARTS];		//!<�p�[�c�̕���
		};

		MotionData m_motion_data;	//!<���[�V�����ǂݍ��݃f�[�^
	};
}

#endif