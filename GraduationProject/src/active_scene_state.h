//=============================================
//
//�Q�[���̃X�e�[�g�p�^�[��[active_scene_state.h]
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
	class CEnemy;

	/** @brief �Q�[���̃X�e�[�g�N���X */
	class CActiveSceneState
	{
	public:
		CActiveSceneState() {};
		virtual ~CActiveSceneState() {};

		/**
		* @brief ���r�[����(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���|�C���^
		*/
		virtual void Lobby([[maybe_unused]] CGame* game) {};


		/**
		* @brief �f���G������(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���|�C���^
		*/
		virtual void Duel([[maybe_unused]] CGame* game) {};

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

	/** @brief ���r�[�N���X */
	class CLobby :public CActiveSceneState
	{
	public:
		static constexpr int SPECIAL_TIME = 30;	//�K�E�Z��ԂɈڂ鎞��
		/**
		* @brief ���r�[����
		* @param [in]�Q�[���v���C���[�|�C���^
		*/
		void Lobby(CGame* game) override;
	private:
		//�����`�Ȃ̂ŃT�C�Y�͓���
		static constexpr float FIELD_SIZE = 200.0f;

		static constexpr int NUM_PLAYER = 4;
		static constexpr float RADIUS = 300.0f;
		/**
		 * @brief ����Ă��Ȃ��v���C���[�𐶐�
		 * @param [in]total
		 * @param [in]enemy
		 */
		void FillEmptyPlayer(int& total, std::list<My::CEnemy*>& enemy);
		/**
		 * @brief �v���C���[����
		 * @param [in]pKeyboard
		 * @param [in]enemy
		 */
		void CreatePlayers(My::CInputKeyboard* pKeyboard, std::list<My::CEnemy*>& enemy);
		/**
		 * @brief ���v���ɔz�u
		 * @param [in]center
		 * @param [in]radius
		 */
		void ArrangePlayerClockwise(const D3DXVECTOR3 center, float radius);
		static int m_characterIdx; //ID TODO:�T�[�o�[���玩�g��ID�擾���邩��^���I�ɃI�t���C���ōČ����邽�߂�static�ɂ��Ă܂��B
	};

	/** @brief �f���G���N���X */
	class CDuel :public CActiveSceneState
	{
	public:
		CDuel();
		~CDuel() override;
		static constexpr int SPECIAL_TIME = 30;	//�K�E�Z��ԂɈڂ鎞��
		/**
		* @brief �f���G������
		* @param [in]�Q�[���v���C���[�|�C���^
		*/
		void Duel(CGame* game) override;
	private:
	};

	/** @brief �|�[�Y��ԏ����N���X */
	class CPause :public CActiveSceneState
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
	class CCardCast :public CActiveSceneState
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
