//============================================
//
// �G�t�F�N�g�}�l�[�W���[[EffectManager.h]
// Author:Uedakou
// 
//============================================
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_
#include <EffekseerForDXLib.h>  // DxLib���܂�
#include "../object.h"
namespace My
{
	class CEffekseerManager : CObject
	{
	public:
		CEffekseerManager();
		~CEffekseerManager();
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		/// <summary>
		/// �G�t�F�N�V�A�}�l�[�W���[�擾
		/// </summary>
		/// <returns>�G�t�F�N�V�A�}�l�[�W���[�̃|�C���^�[</returns>
		Effekseer::ManagerRef GetManager() {
			return m_managerRef;
		}
		/// <summary>
		/// �G�t�F�N�V�A�����_�[�̎擾
		/// </summary>
		/// <returns>�G�t�F�N�V�A�����_�[�̃|�C���^�[</returns>
		EffekseerRenderer::RendererRef GetRenderer() {
			return m_rendererRef;
		}

	private:
		Effekseer::ManagerRef m_managerRef;
		EffekseerRenderer::RendererRef m_rendererRef;
	};


}

#endif // !_EFFECT_MANAGER_H_