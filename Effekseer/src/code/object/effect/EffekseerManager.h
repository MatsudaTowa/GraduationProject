//============================================
//
// エフェクトマネージャー[EffectManager.h]
// Author:Uedakou
// 
//============================================
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_
#include <EffekseerForDXLib.h>  // DxLibも含む
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
		/// エフェクシアマネージャー取得
		/// </summary>
		/// <returns>エフェクシアマネージャーのポインター</returns>
		Effekseer::ManagerRef GetManager() {
			return m_managerRef;
		}
		/// <summary>
		/// エフェクシアレンダーの取得
		/// </summary>
		/// <returns>エフェクシアレンダーのポインター</returns>
		EffekseerRenderer::RendererRef GetRenderer() {
			return m_rendererRef;
		}

	private:
		Effekseer::ManagerRef m_managerRef;
		EffekseerRenderer::RendererRef m_rendererRef;
	};


}

#endif // !_EFFECT_MANAGER_H_