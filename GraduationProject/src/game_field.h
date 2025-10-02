//=============================================
//
//�Q�[���̏�[game_field.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_FIELD_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _GAME_FIELD_H_
#include "main.h"
#include "field.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �Q�[�����N���X
	 * @copydoc CField
	 */
	class CGameField : public CField
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CGameField(int nPriority = FIELD_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CGameField()override;

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
	private:
		static const std::string TEXTURE_NAME;	//!<�e�N�X�`���̖��O
	};
}

#endif