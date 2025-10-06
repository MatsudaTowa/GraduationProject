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

		struct Status
		{
			int deckSize;	//!<�f�b�L�̖���
			int trash;		//!<��n����
			int hand;		//!<��D����
			int energy;		//!<�G�i�W�[
			int life;		//!<�̗�
		};

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
		* @brief �X�e�[�^�X�ݒ�
		* @param [in]�X�e�[�^�X
		*/
		inline void SetStatus(Status status)
		{
			m_status = status;
		}

		/**
		 * @brief �f�b�L�����ݒ�
		 * @param [in]�f�b�L�̖���
		 */
		inline void SetDeck(int decksize)
		{
			m_status.deckSize = decksize;
		}

		/**
		 * @brief ��n�����ݒ�
		 * @param [in]��n����
		 */
		inline void SetTrash(int trash)
		{
			m_status.trash = trash;
		}

		/**
		 * @brief �n���h�ݒ�
		 * @param [in]�n���h����
		 */
		inline void SetHand(int hand)
		{
			m_status.hand = hand;
		}

		/**
		 * @brief �̗͐ݒ�
		 * @param [in]�̗�
		 */
		inline void SetLife(int life)
		{
			m_status.life = life;
		}

		/**
		 * @brief �G�i�W�[�ݒ�
		 * @param [in]�G�i�W�[
		 */
		inline void SetEnergy(int energy)
		{
			m_status.energy = energy;
		}

		/**
		 * @brief �̗�UI�̐ݒ�
		 * @param [in]�̗�UI�̃|�C���^
		 */
		inline void SetLifeUI(CLife_UI* plifeUI)
		{
			m_pLifeUI = plifeUI;
		}

		/**
		 * @brief �X�e�[�^�X�擾
		 * @return �X�e�[�^�X
		 */
		inline Status GetStatus()
		{
			return m_status;
		}

		/**
		 * @brief �f�b�L�����擾
		 * @return �f�b�L����
		 */
		inline int GetDeck()
		{
			return m_status.deckSize;
		}

		/**
		 * @brief ��n�����擾
		 * @return ��n����
		 */
		inline int GetTrash()
		{
			return m_status.trash;
		}

		/**
		 * @brief �n���h�擾
		 * @return �n���h����
		 */
		inline int GetHand()
		{
			return m_status.hand;
		}

		/**
		 * @brief �̗͎擾
		 * @return �̗�
		 */
		inline int GetLife()
		{
			return m_status.life;
		}

		/**
		 * @brief �G�i�W�[�擾
		 * @return �G�i�W�[
		 */
		inline int GetEnergy()
		{
			return m_status.energy;
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

		/**
		 * @brief �̗�UI�̎擾
		 * @return [in]�̗�UI�̃|�C���^
		 */
		inline CLife_UI* GetLifeUI()
		{
			return m_pLifeUI;
		}
	private:
		static constexpr int START_DECK = 40;				//!<�ŏ��̃f�b�L����
		static constexpr int START_HAND = 3;				//!<�ŏ��̎�D����
		static constexpr int START_LIFE = 10;				//!<�ŏ��̗̑�
		static constexpr int START_ENERGY = 1;				//!<�ŏ��̃G�i�W�[
		static constexpr int MAX_ENERGY = 99;				//!<�G�i�W�[�ő�l
		static constexpr int MAX_LIFE = 99;					//!<�̗͍ő�l

		static constexpr float SHADOW_POS_Y = 0.5f;	 //!<�e��Y���W(�n�ʂ��班��������)

		bool m_isLoopFinish;		//!<���[�v���[�V�������I�������
		int m_PartsCnt;				//!<�p�[�c��
		CCount* m_pMotionFrameCnt;	//!���[�V�����t���[���J�E���g
		int m_nKeySetCnt;			//!<�L�[�J�E���g
		int m_Motion;				//!<���[�V����(�e�I�u�W�F�N�g����񋓂��󂯎��)
		D3DXCOLOR m_col;			//!<�J���[
		D3DXVECTOR3 m_ShadowSize;	//!<�e�̃T�C�Y

		Status m_status;			//!�X�e�[�^�X

		CShadow* m_pShadow;			//!<�e

		CLife_UI* m_pLifeUI;		//!<�̗�UI TODO:UI�̃C���X�^���X���L�����N�^�[�Ŏ������Ȃ��悤�ɉ���

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