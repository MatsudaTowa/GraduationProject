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

/** @brief My ���O��� */
namespace My
{
	/** @brief �L�����N�^�[�N���X */
	class CCharacter : public CObjectX
	{
	public:
		static const int MAX_KEY = 20;					//!<�L�[�ő吔
		static const int CHARACTER_PRIORITY = 8;		//!<�L�����N�^�[�̃v���C�I���e�B
		static const int MAX_MOTION = 100;				//!<���[�V�����̍ő吔
		static const int MAX_PARTS = 64;				//!<�ő�p�[�c��
		static constexpr float KILL_Y = -200.0f;		 //!<��������������E��

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
		 * @brief �d��
		 */		
		void Gravity();

		/**
		 * @brief �W�����v����
		 */		
		void Jump();

		/**
		 * @brief ���̔���
		 */		
		void HitField();

		/**
		 * @brief ���[���h�}�g���b�N�X�ɕϊ�
		 */		
		void ConversionMtxWorld();

		/**
		 * @brief �ړ��ʐݒ�
		 * @param [in]�ړ���
		 */		
		inline void SetMove(D3DXVECTOR3 move)
		{
			m_move = move;
		}

		/**
		 * @brief �ߋ��̈ʒu�ݒ�
		 * @param [in]�ߋ��̈ʒu
		 */
		inline void SetOldPos(D3DXVECTOR3 oldpos)
		{
			m_oldpos = oldpos;
		}

		/**
		 * @brief ���n���Ă��邩�ݒ�
		 * @param [in]�ߋ��̈ʒu
		 */
		inline void SetLanding(bool bLanding)
		{
			m_bLanding = bLanding;
		}

		/**
		 * @brief ���C�t�ݒ�
		 * @param [in]���݂̗̑�
		 */
		inline void SetLife(int nLife)
		{
			m_nLife = nLife;
		}

		/**
		 * @brief �W�����v���ݒ�
		 * @param [in]�W�����v��
		 */		
		inline void SetJumpCnt(int nJumpCnt)
		{
			m_nJumpCnt = nJumpCnt;
		}

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
		  * @brief �ړ��ʎ擾
		  * @return �ړ���
		  */		
		inline D3DXVECTOR3& GetMove()
		{
			return m_move;
		}

		 /**
		  * @brief �ߋ��Ɉʒu�擾
		  * @return �ߋ��̈ʒu
		  */	
		inline D3DXVECTOR3& GetOldPos()
		{
			return m_oldpos;
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
		 * @brief ���n���Ă��邩�擾
		 * @return ���n���Ă���
		 */		
		inline bool& GetLaunding()
		{
			return m_bLanding;
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
		 * @brief �̗͎擾
		 * @return ���݂̗̑�
		 */				
		inline int& GetLife()
		{
			return m_nLife;
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
		 * @brief ���x�擾
		 * @return ���݂̑��x
		 */			
		inline float& GetSpeed()
		{
			return m_Speed;
		}

		/**
		 * @brief �W�����v���擾
		 * @return ���݂̃W�����v��
		 */	
		inline int& GetJumpCnt()
		{
			return m_nJumpCnt;
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
		static constexpr float MOVE_FRICTION = 0.3f; //!<�ړ���R
		static constexpr float SHADOW_POS_Y = 0.5f;	 //!<�e��Y���W(�n�ʂ��班��������)
		static constexpr float GRAVITY_MOVE = 2.0f;  //!<�d�͒l
		static constexpr float GRAVITY_MAX = 100.0f; //!<�d�͍ő�l

		D3DXVECTOR3 m_move;			//!<���x
		D3DXVECTOR3 m_oldpos;		//!<�ߋ��̈ʒu
		bool m_bLanding;			//!<���n���Ă邩�ǂ���
		bool m_isLoopFinish;		//!<���[�v���[�V�������I�������
		int m_nLife;				//!<�̗�
		int m_PartsCnt;				//!<�p�[�c��
		int m_nMotionFrameCnt;		//!<�؂�ւ��t���[���J�E���g
		int m_nKeySetCnt;			//!<�L�[�J�E���g
		int m_nJumpCnt;				//!<�W�����v�J�E���g
		int m_Motion;				//!<���[�V����(�e�I�u�W�F�N�g����񋓂��󂯎��)
		float m_Speed;				//!<�X�s�[�h
		float m_Jump;				//!<�W�����v
		D3DXCOLOR m_col;			//!<�J���[
		D3DXVECTOR3 m_ShadowSize;	//!<�e�̃T�C�Y

		CShadow* m_pShadow;			//!<�e

		CModel_Parts* m_apModel[MAX_PARTS]; 		//!<�p�[�c

		/**
		 * @brief �L�[�\����
		 */	
		struct Key
		{
			D3DXVECTOR3 s_pos;		//!<�ʒu
			D3DXVECTOR3 s_rot;		//!<����
			D3DXVECTOR3 s_Trot;		//!<T�|�[�Y���̕���
		};

		/**
		 * @brief �L�[�ݒ�\����
		 */	
		struct KeySet
		{
			int s_nFrame;			//!<�t���[����
			Key s_key[MAX_KEY];		//!<�L�[���
		};

		/**
		 * @brief ���[�V�����ݒ�\����
		 */	
		struct MotionSet
		{
			int s_nLoop;					//!<���[�v���邩�ǂ���
			int s_nNumKey;					//!<�L�[��
			KeySet s_keySet[MAX_PARTS];		//!<�L�[�ݒ���
		};

		/**
		 * @brief ���[�V�����ǂݍ��ݏ��\����
		 */
		struct MotionData
		{
			MotionSet s_motion_set[MAX_MOTION];		//!<���[�V�����ݒ�
			int s_parts;							//!<�p�[�c��
			int s_idx[MAX_PARTS];					//!<�p�[�c�ԍ�
			int s_parent[MAX_PARTS];				//!<�e�̔ԍ�
			float s_speed;							//!<���x
			float s_jump;							//!<�W�����v��
			char s_path[MAX_PARTS][256];		//!<�t�@�C���p�X
			D3DXVECTOR3 s_parts_pos[MAX_PARTS];		//!<�p�[�c�̈ʒu
			D3DXVECTOR3 s_parts_rot[MAX_PARTS];		//!<�p�[�c�̕���
		};

		MotionData m_motion_data;	//!<���[�V�����ǂݍ��݃f�[�^
	};
}

#endif