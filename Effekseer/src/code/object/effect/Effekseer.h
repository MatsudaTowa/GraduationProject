//============================================
//
// �G�t�F�N�g[Effekseer.h]
// Author:Uedakou
// 
//============================================
#ifndef EFFEKSEER_H_
#define EFFEKSEER_H_

#include <EffekseerForDXLib.h>  // DxLib���܂�
#include "../object.h"

namespace My
{
	// �G�t�F�N�g�Ǘ��N���X
	class Effect : public CObject
	{
	public:
		Effect();			// �R���X�g���N�^
		~Effect();			// �f�X�g���N�^
		HRESULT Init();		// ������
		void Uninit();		// �I��
		void Update();		// �X�V
		void Draw();		// �`��

		//void SetPos(::D3DXVECTOR3 pos);

		static Effect* create(const std::string sFilepas);
	private:
		Effekseer::EffectRef g_effectRef = nullptr;
		Effekseer::Handle g_handle;

		// �ϐ�
		const int	EffectPlayInterval = 300;					// �G�t�F�N�g���Đ��������

		// ����̓���łɂݕK�v�ȕϐ�
		int		playCount;			// �����G�ɍĐ����邽�߂̃J�E���g
	};

}
#endif // !EFFEKSEER_H_