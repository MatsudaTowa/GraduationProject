//=============================================
//
//�����蔻��Ǘ�[colision.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COLISION_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _COLISION_H_

/** @brief My ���O��� */
namespace My
{
	/** @brief �����蔻��N���X */
	class CColision
	{
	public:
		/** @brief �ǂ��ɓ����������̗� */
		enum COLISION
		{
			COLISON_NONE = 0,	//!<���ɂ��������ĂȂ�
			COLISON_X,			//!<X�����ɓ������Ă�
			COLISON_Z,			//!<Z�����ɓ������Ă�
			COLISON_UNDER_Y,	//!<Y����(��)�ɓ������Ă�
			COLISON_TOP_Y,		//!<Y����(��)�������Ă�
			COLISION_CIRCLE,	//!<�~��ɓ������Ă���
		};

		/** @brief �~�̔�����\���� */
		struct CIRCLE
		{
			COLISION colision;		//!<�ǂ�����������
			float CenterDistance;	//!<���S�_�Ƃ̋���
		};

		/**
		 * @brief �R���X�g���N�^
		 */		
		CColision();

		/**
		 * @brief �f�X�g���N�^
		 */			
		~CColision();

		/**
		 * @brief X���̃|���S�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */
		COLISION CheckColision_X_2D(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Bpos, D3DXVECTOR2 Bsize);

		/**
		 * @brief X���̃|���S�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */	
		COLISION CheckColision_X(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief X���̃��f�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */
		COLISION CheckColision_X(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Y���̃|���S�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */		
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Y���̃��f���ƃ|���S�������蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Y���̃��f�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Z���̃|���S�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Z���̃��f���ƃ|���S�������蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Z���̃��f�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief ���̃��f�����m�����蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̉ߋ��̈ʒu
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̍ŏ��l
		 * @param [in]�����鑤�̍ő�l
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */
		COLISION CheckColisionSphere(D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief �~��̓����蔻��
		 * @param [in]�����鑤�̈ʒu
		 * @param [in]���a
		 * @param [in]�������鑤�̈ʒu
		 * @return �~�̃q�b�g���
		 */
		CIRCLE CheckColisionCircle(D3DXVECTOR3 Apos, float Radius, D3DXVECTOR3 Bpos);

		/**
		 * @brief �|���S���ƃ��f�������蔻��`�F�b�N�֐�_��
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */		
		COLISION CheckPolygonModelColisionSphere(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);


		/**
		 * @brief X���̃|���S���ƃ��f�������蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */
		COLISION CheckPolygonModelColision_X(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Z���̃|���S���ƃ��f�������蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */		
		COLISION CheckPolygonModelColision_Z(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief �|���S���ƃ��f�������蔻��(���܂��Ă邩)�`�F�b�N�֐�
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̍ŏ��l
		 * @param [in]�������鑤�̍ő�l
		 * @return �������Ă��邩
		 */
		COLISION CheckPolygonFillColision(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);
		
		/**
		 * @brief 2D�|���S�������蔻��`�F�b�N�֐�
		 * @param [in]�����鑤�̌��݂̈ʒu
		 * @param [in]�����鑤�̃T�C�Y
		 * @param [in]�������鑤�̈ʒu
		 * @param [in]�������鑤�̃T�C�Y
		 * @return �������Ă��邩
		 */
		bool Check2DPolygonColision(D3DXVECTOR3 Apos, D3DXVECTOR2 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR2 BSize)
		{
			if (Apos.x - ASize.x < Bpos.x + BSize.x
				&& Apos.x + ASize.x > Bpos.x - BSize.x
				&& Apos.y - ASize.y < Bpos.y + BSize.y
				&& Apos.y + ASize.y > Bpos.y - BSize.y)
			{
				return true;
			}

			return false;
		}
	};
}

#endif