//=============================================
//
//���p�I�ȏ������܂Ƃ߂�����[useful.h]
//Author Matsuda Towa
//
//============================================= 

//�Q�d�C���N���[�h�h�~
#ifndef _USEFUL_H_ //���ꂪ��`����ĂȂ��Ƃ�
#define _USEFUL_H_

#include "main.h"
#include "manager.h"
#include "camera.h"

//0�������p
#define INT_ZERO (0)
#define FLOAT_ZERO (0.0f)
#define INT_ONE (1)
#define FLOAT_ONE (1.0f)

//��b
#define ONE_SECOND (60)

//����
#define HALF	(0.5f)

//�J���[������
#define COLOR_NONE (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define COLOR_BLACK (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define COLOR_WHITE (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define COLOR_RED (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define COLOR_GREEN (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))
#define COLOR_BLUE (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))
#define COLOR_YELLOW (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define COLOR_ORANGE (D3DXCOLOR(1.0f,0.5f,0.0f,1.0f))
#define COLOR_PURPLE (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))
#define COLOR_BLOWN (D3DXCOLOR(0.7f,0.3f,0.0f,1.0f))

//���_3D������
#define VEC3_RESET_ZERO (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define VEC3_RESET_ONE (D3DXVECTOR3(1.0f,1.0f,1.0f))

//���_2D������
#define VEC2_RESET_ZERO (D3DXVECTOR2(0.0f,0.0f))
#define VEC2_RESET_ONE (D3DXVECTOR2(1.0f,1.0f))

//���~
#define HALF_PI	(D3DX_PI * HALF)
//1/4�~
#define QRTR_PI	(D3DX_PI * (HALF * HALF))

//�������ő吔
#define MAX_TXT (256)

//���ˏ�Ƀ����_��
#define CICRLE_RAND  ((float)(rand() % 629 - 314) / 1e2f)

// �}�l�[�W���[�֌W
#define GET_MANAGER				(My::CManager::GetInstance())								// �}�l�[�W���[�C���X�^���X�擾
#define GET_SCENE				(GET_MANAGER->GetScene())									// �V�[���擾
#define GET_SCORE				(GET_MANAGER->GetScore())									// �X�R�A�擾
#define GET_CAMERA(idx)			(GET_MANAGER->GetCamera(idx))								// �J�����擾
#define ERASE_CAMERA(idx)		(GET_MANAGER->EraseCamera(idx))								// �w�肳�ꂽ�J�����폜
#define CREATE_CAMERA(camera)	(GET_MANAGER->CreateCamera(camera))							// �J��������
#define GET_CAMER_SIZE			(GET_MANAGER->GetCameraSize())								// �J�����T�C�Y
#define CLEAR_CAMERA			(GET_MANAGER->ClearCamera())								// �J�����S���폜
#define GET_CAMERA_IDX			(GET_MANAGER->GetCureentViewCamera())						// �f���Ă���J�����̔ԍ��擾
#define SET_CAMERA_IDX(idx)		(GET_MANAGER->SetCureentViewCamera(idx))					// �f���Ă���J�����̔ԍ��ݒ�
#define GET_LIGHT				(GET_MANAGER->GetLight())									// ���C�g�擾
#define GET_MODEL				(GET_MANAGER->GetModel())									// ���f���擾
#define GET_TEXTURE				(GET_MANAGER->GetTexture())									// �e�N�X�`���擾
#define GET_FADE				(GET_MANAGER->GetFade())									// �t�F�[�h�擾
#define GET_INPUT_KEYBOARD		(GET_MANAGER->GetKeyboard())								// �L�[�{�[�h���擾
#define GET_INPUT_MOUSE			(GET_MANAGER->GetMouse())									// �}�E�X���擾
#define GET_INPUT_PAD			(GET_MANAGER->GetPad())										// �p�b�h���擾
#define GET_RENDERER			(GET_MANAGER->GetRenderer())								// �����_���[���擾
#define GET_FONT				(GET_MANAGER->GetFont())									// �t�H���g�擾
#define GET_DEVICE				(GET_MANAGER->GetRenderer()->GetDevice())					// �f�o�C�X���擾
#define GET_SOUND				(GET_MANAGER->GetSound())									// �T�E���h�擾
#define GET_COLISION			(GET_MANAGER->GetColision())								// �����蔻��擾

//�C���N�������g�֐�
template <std::floating_point T>  // float, double �ȂǕ��������_�^�̂�
void Increment(T& value)
{
    value += 1.0;
}

auto Rundom(auto min, auto max)
{
	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

/**
* @brief �X�N���[�����W�����[���h���W�ɕϊ�
* @param [in][out]��_�̃|�C���^
* @param [in]�X�N���[��X���W
* @param [in]�X�N���[��Y���W
* @param [in]�ˉe��Ԃł�Z�l�i0�`1)
* @param [in]�X�N���[���̕�
* @param [in]�X�N���[���̍���
* @param [in]�r���[�}�g���b�N�X
* @param [in]�v���W�F�N�V�����}�g���b�N�X
*/
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	float Sx,
	float Sy,
	float fZ,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
* @param [in][out]��_�̃|�C���^
* @param [in]�q�b�g����Ώۂ̃I�u�W�F�N�g���W
* @param [in]�X�N���[��X���W
* @param [in]�X�N���[��Y���W
* @param [in]�X�N���[���̕�
* @param [in]�X�N���[���̍���
* @param [in]�r���[�}�g���b�N�X
* @param [in]�v���W�F�N�V�����}�g���b�N�X
*/
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,
	D3DXVECTOR3 hit_obj,
	float Sx,
	float Sy,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief XY���ʂƃX�N���[�����W�̌�_�Z�o�֐�
* @param [in][out]��_�̃|�C���^
* @param [in]�q�b�g����Ώۂ̃I�u�W�F�N�g���W
* @param [in]�X�N���[��X���W
* @param [in]�X�N���[��Y���W
* @param [in]�X�N���[���̕�
* @param [in]�X�N���[���̍���
* @param [in]�r���[�}�g���b�N�X
* @param [in]�v���W�F�N�V�����}�g���b�N�X
*/
D3DXVECTOR3* CalcScreenToXY(
	D3DXVECTOR3* pout,
	D3DXVECTOR3 hit_obj,
	float Sx,
	float Sy,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief �^�[�Q�b�g�ɓ��B������
* @param [in]���B臒l
* @param [in]�X�s�[�h
* @param [in]����
* @param [in]����
* @param [in]�ړ���
* @return ���B������
*/
bool isTargetReached(const float threshold,float speed, float& angle, float& distance, D3DXVECTOR3& move);

/**
 * @brief ���[���h���W�ɕϊ�
 * @param [in]�ʒu
 * @param [in]���C�𓖂Ă�ʒu
 * @return �ϊ���̒l
 */
D3DXVECTOR3 ConvertToWorldPoint(My::CCamera* camera, D3DXVECTOR3 pos,D3DXVECTOR3 hit_pos);
/**
 * @brief �X�N���[�����W�ɕϊ�
 * @param [in]�ʒu
 * @return �ϊ���̒l
 */
D3DXVECTOR3 ConvertToScreenPos(My::CCamera* camera,D3DXVECTOR3 pos);

#endif