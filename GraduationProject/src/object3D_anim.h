//=============================================
//
//�A�j���[�V��������3D�I�u�W�F�N�g[object3D_anim.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT_3D_ANIM_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _OBJECT_3D_ANIM_H_
#include "main.h"
#include "object3D.h"
#include "count.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �A�j���[�V����3D�I�u�W�F�N�g�N���X */
	class CObject3D_Anim : public CObject3D
	{
	public:
		CObject3D_Anim(int nPriority);
		~CObject3D_Anim()override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		//�A�j���[�V��������
		void AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move);
		//�A�j���[�V�����t���[�����
		inline void SetAnimFrame(int nAnimFrame)
		{
			m_pCnt->SetFrame(nAnimFrame);
		}

		//�e�N�X�`���ړ��ʂ̑��
		inline void SetTexMove(D3DXVECTOR2 tex_move)
		{
			m_tex_move = tex_move;
		}

		//�A�j���[�V�����t���[���擾
		inline int GetAnimFrame()
		{
			return m_pCnt->GetFrame();
		}

		//�e�N�X�`���ړ��ʎ擾
		inline D3DXVECTOR2& GetTexMove()
		{
			return m_tex_move;
		}

	private:
		CCount* m_pCnt;
		D3DXVECTOR2 m_tex_move; //tex�ړ���

	};
}
#endif