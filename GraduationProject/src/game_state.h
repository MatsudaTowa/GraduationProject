//=============================================
//
//�Q�[���̃X�e�[�g�p�^�[��[game_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CGame;

	/** @brief �Q�[���̃X�e�[�g�N���X */
	class CGameState
	{
	public:
		CGameState() {};
		virtual ~CGameState() {};
		/**
		* @brief �ʏ폈��(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���|�C���^
		*/
		virtual void Normal([[maybe_unused]] CGame* game) {};

		/**
		* @brief �|�[�Y��ԏ���(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���|�C���^
		*/
		virtual void Pause([[maybe_unused]] CGame* game) {};

		/**
		* @brief �|�[�Y��ԏ���(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���|�C���^
		*/
		virtual void CardCast([[maybe_unused]] CGame* game) {};
	};

	/** @brief �ʏ�N���X */
	class CNormal :public CGameState
	{
	public:
		static constexpr int SPECIAL_TIME = 30;	//�K�E�Z��ԂɈڂ鎞��
		/**
		* @brief �j���[�g��������
		* @param [in]�Q�[���v���C���[�|�C���^
		*/
		void Normal(CGame* game) override;
	private:
	};

	/** @brief �|�[�Y��ԏ����N���X */
	class CPause :public CGameState
	{
	public:
		~CPause()override;
		/**
		* @brief �|�[�Y��ԏ���
		* @param [in]�Q�[���|�C���^
		*/
		void Pause(CGame* game) override;
	private:
		static constexpr float THRESHOLD = 10.0f; // �������萔�ȉ��Ȃ瓞�B�Ƃ���i�K�v�ɉ����Ē����j
	};

	/** @brief �J�[�h�L���X�g�����N���X */
	class CCardCast :public CGameState
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CCardCast();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CCardCast()override;
		/**
		* @brief �J�[�h��ԏ���
		* @param [in]�Q�[���|�C���^
		*/
		void CardCast(CGame* game) override;
	private:
	};
}
#endif // !_GAME_PLAYER_STATE_H_
