//=============================================
//
//�����Ǘ��unumber.h�v
// Author���c�i�v
//
//=============================================
#ifndef _NUMBER_H_ //���ꂪ��`����ĂȂ��Ƃ�
#define _NUMBER_H_
#include "main.h"
#include "object2D.h"
#include "billboard.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief My ����(2D)�N���X */
	class CNumber_2D :public CObject2D
	{
	public:
		static const int NUMBER_PRIORITY = 29;  //�`�揇

		enum TEX_TYPE
		{
			TEX_000 = 0,
			TEX_001,
			MAX
		};

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CNumber_2D(int nPriority = NUMBER_PRIORITY);
		/**
		 * @brief �f�X�g���N�^
		 */
		~CNumber_2D()override;
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
		 * @brief ��������
		 * @param [in]�ʒu
		 * @param [in]�T�C�Y
		 * @param [in]�e�N�X�`���ԍ�
		 * @return ������2D�|�C���^
		 */
		static CNumber_2D* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int idx);

		/**
		 * @brief �����ݒ�
		 * @param [in]�����̃e�N�X�`��min���W
		 * @param [in]�����̃e�N�X�`��max���W
		 * @param [in]�F
		 */
		void SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col);

		/**
		 * @brief �����̒��_����
		 */
		void NumberVtx();

	private:
		D3DXVECTOR2 m_mintex; //�e�N�X�`���̍ŏ����W
		D3DXVECTOR2 m_maxtex; //�e�N�X�`���̍ő���W
	};

	/** @brief My ����(3D)�N���X */
	class CNumber_3D :public CBillboard
	{
	public:
		static const int NUMBER_PRIORITY = 20;  //�`�揇
		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CNumber_3D(int nPriority = NUMBER_PRIORITY);
		/**
		 * @brief �f�X�g���N�^
		 */
		~CNumber_3D()override;
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
		 * @brief ��������
		 * @param [in]�ʒu
		 * @param [in]�T�C�Y
		 * @return ������3D�|�C���^
		 */		
		static CNumber_3D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

		/**
		 * @brief �����̐ݒ�
		 * @param [in]�����̃e�N�X�`��min���W
		 * @param [in]�����̃e�N�X�`��max���W
		 * @param [in]�F
		 */
		void SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col);

		/**
		 * @brief �����̒��_����
		 */
		void NumberVtx();

	private:
		static const std::string TEXTURE_NAME;	//�e�N�X�`���̖��O

		D3DXVECTOR2 m_mintex; //�e�N�X�`���̍ŏ����W
		D3DXVECTOR2 m_maxtex; //�e�N�X�`���̍ő���W
	};
}

#endif // NUMBER