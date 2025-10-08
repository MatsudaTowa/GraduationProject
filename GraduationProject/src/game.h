//=============================================
//
//�Q�[������[game.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _GAME_H_
#include "main.h"
#include "scene.h"
#include "player.h"
#include "game_state.h"
#include "count.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �Q�[���V�[���N���X
	 * @copydoc CScene
	 */
	class CGame :public CScene
	{
	public:
		enum CAMERA_TYPE
		{
			GAME_CAMERA = 0,
			EVENT_CAMERA_000,
			EVENT_CAMERA_001,
			BOARD_GENERATE_CAMERA,
			CAMERA_MAX,
		};
		/**
		 * @brief �R���X�g���N�^
		 */
		CGame();

		/**
		 * @brief �f�X�g���N�^
		 */	
		~CGame() override;

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
		 * @brief �G���A�̑I��
		 */
		void SelectArea();

		/**
		 * @brief �I�u�W�F�N�g�̍X�V���s��������
		 * @param [in]�~�߂邩�ǂ���
		 */
		void StopObject(bool isStop);

		/**
		 * @brief �`��
		 */
		void Draw() override;

		/**
		 * @brief ���͉\���擾
		 * @param ���͉\��
		 */
		inline bool GetPauseKey()
		{
			if (m_pPauseCnt == nullptr)
			{
				return false;
			}
			return m_pPauseCnt->CountUp();
		}

		/**
		 * @brief �J�E���g���Z�b�g
		 */
		inline void ResetPauseCnt()
		{
			if (m_pPauseCnt == nullptr)
			{
				return;
			}
			m_pPauseCnt->SetCnt(0);
		}
	private:
		CCount* m_pPauseCnt;					//�|�[�Y�̃L�[��L�������邽�߂̃J�E���g
		CCount* m_pDelayCnt;					//���U���g�ɑJ�ڂ���܂ł̃f�B���C�J�E���g
	};
}

#endif