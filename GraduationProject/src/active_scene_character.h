//=============================================
//
//�A�N�e�B�u�V�[���L�����N�^�[[achtive_scene_character.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ACTIVE_SCENE_CHARACTER_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _ACTIVE_SCENE_CHARACTER_H_
#include "main.h"
#include "character.h"
#include "active_scene_character_state.h"
/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �A�N�e�B�u�V�[���L�����N�^�[[
	 * @copydoc CCharacter
	 */
	class CActiveSceneCharacter :public CCharacter
	{
	public:
		static constexpr int PRIORITY = 8;		//!<�`�揇

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
		 * @param �v���C�I���e�B
		 */
		CActiveSceneCharacter(int nPriority = PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CActiveSceneCharacter() override;

		/**
		 * @brief ������
		 * @return ����������
		 */
		HRESULT Init() override;

		/**
		 * @brief �I��
		 */
		void Uninit() override;

		/**
		 * @brief �X�V
		 */
		void Update() override;

		/**
		 * @brief �`��
		 */
		void Draw() override;

		/**
		 * @brief �X�e�[�g�ύX
		 * @param [in]���̃X�e�[�g
		 */
		void ChangeState(CActiveSceneCharacterState* state);

		/**
		 * @brief �X�e�[�g�擾
		 * @return �X�e�[�g�|�C���^
		 */
		inline CActiveSceneCharacterState* GetState()
		{
			return m_pState;
		}

		/**
		 * @brief �v���C���[�ԍ����
		 * @param [in]playerIdx
		 */
		inline void SetPlayerIdx(int playerIdx)
		{
			m_playerIdx = playerIdx;
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
		 * @brief �G�i�W�[UI�̐ݒ�
		 * @param [in]�G�i�W�[UI�̃|�C���^
		 */
		inline void SetEnergyUI(CEnergy_UI* pEnergyUI)
		{
			m_pEneryUI = pEnergyUI;
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
		 * @brief �v���C���[�̔ԍ��擾
		 * @return idx
		 */
		inline int GetPlayerIdx()
		{
			return m_playerIdx;
		}

		/**
		 * @brief �̗�UI�̎擾
		 * @return [in]�̗�UI�̃|�C���^
		 */
		inline CLife_UI* GetLifeUI()
		{
			return m_pLifeUI;
		}

		/**
		 * @brief �G�i�W�[UI�̎擾
		 * @return [in]�G�i�W�[UI�̃|�C���^
		 */
		inline CEnergy_UI* GetEnergyUI()
		{
			return m_pEneryUI;
		}
	private:
		static constexpr int START_DECK = 40;				//!<�ŏ��̃f�b�L����
		static constexpr int START_HAND = 3;				//!<�ŏ��̎�D����
		static constexpr int START_LIFE = 10;				//!<�ŏ��̗̑�
		static constexpr int START_ENERGY = 1;				//!<�ŏ��̃G�i�W�[
		static constexpr int MAX_ENERGY = 99;				//!<�G�i�W�[�ő�l
		static constexpr int MAX_LIFE = 99;					//!<�̗͍ő�l

		Status m_status;			//!�X�e�[�^�X

		int m_playerIdx = -1;

		CLife_UI* m_pLifeUI;			//!<�̗�UI TODO:UI�̃C���X�^���X���L�����N�^�[�Ŏ������Ȃ��悤�ɉ���
		CEnergy_UI* m_pEneryUI;			//!�G�i�W�[UI TODO:UI�̃C���X�^���X���L�����N�^�[�Ŏ������Ȃ��悤�ɉ���
		CActiveSceneCharacterState* m_pState;	//!�X�e�[�g
	};
}
#endif