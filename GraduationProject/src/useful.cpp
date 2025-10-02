#include "useful.h"
//=============================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=============================================
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w * 0.5f;
	VP._22 = -Screen_h * 0.5f;
	VP._41 = Screen_w * 0.5f;
	VP._42 = Screen_h * 0.5f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 screenpos(Sx, Sy, fZ);
	D3DXVec3TransformCoord(pout, &screenpos, &tmp);
	return pout;
}

//=============================================
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//=============================================
D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* pout, D3DXVECTOR3 hit_obj, float Sx, float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= hit_obj.y) 
	{
		D3DXVECTOR3 nor = { 0.0f, 1.0f, 0.0f };
		// ����_
		float Lray = D3DXVec3Dot(&ray, &nor);
		D3DXVECTOR3 inv_nearpos = -nearpos;
		float LP0 = D3DXVec3Dot(&inv_nearpos, &nor);
		*pout = nearpos + (LP0 / Lray) * ray;
	}
	else
	{
		*pout = farpos;
	}

	return pout;
}

D3DXVECTOR3* CalcScreenToXY(D3DXVECTOR3* pout, D3DXVECTOR3 hit_obj, float Sx, float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.z <= hit_obj.z)
	{
		D3DXVECTOR3 nor = { 0.0f, 0.0f, -1.0f };
		// ����_
		float Lray = D3DXVec3Dot(&ray, &nor);
		D3DXVECTOR3 inv_nearpos = -nearpos;
		float LP0 = D3DXVec3Dot(&inv_nearpos, &nor);
		*pout = nearpos + (LP0 / Lray) * ray;
	}
	else
	{
		*pout = farpos;
	}

	return pout;
}

//=============================================
//���B���Ă��邩�ǂ���
//=============================================
bool isTargetReached(const float threshold, float speed, float& angle, float& distance, D3DXVECTOR3& move)
{
	// �܂��ړI�n�ɓ��B���Ă��Ȃ��ꍇ�݈̂ړ��������s��
	if (distance > threshold)
	{
		move.x += sinf(angle) * speed;
		move.z += cosf(angle) * speed;
		return false;
	}

	return true;
}

//=============================================
//���[���h���W�ϊ�
//=============================================
D3DXVECTOR3 ConvertToWorldPoint(My::CCamera* camera,D3DXVECTOR3 pos, D3DXVECTOR3 hit_pos)
{
	D3DXVECTOR3 convert_pos = VEC3_RESET_ZERO;

	D3DXMATRIX view_mtx = camera->GetViewMtx();
	D3DXMATRIX prj_mtx = camera->GetPrjMtx();

	CalcScreenToXZ(&convert_pos, hit_pos, pos.x, pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, &view_mtx, &prj_mtx);
	
	return convert_pos;
}

//=============================================
//�X�N���[�����W�ϊ�
//=============================================
D3DXVECTOR3 ConvertToScreenPos(My::CCamera* camera, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 screen_pos = VEC3_RESET_ZERO;

	D3DVIEWPORT9 view_port = camera->GetViewPort();

	D3DXMATRIX prj_mtx = camera->GetPrjMtx();

	D3DXMATRIX view_mtx = camera->GetViewMtx();

	//NOTE: �����`��Ə������Ń��[���h�}�g���b�N�X�ɕϊ����Ă�̂�
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	D3DXVec3Project(&screen_pos, &pos, &view_port, &prj_mtx, &view_mtx, &identity);

	return screen_pos;
}
