//============================================
//
// エフェクトマネージャー[EffectManager.h]
// Author:Uedakou
// 
//============================================
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_
#include <EffekseerForDXLib.h>  // DxLibも含む
#include "object.h"				// オブジェクト
namespace My
{
	class CEffectManager
	{
	public:
		CEffectManager();		// コンストラクタ
		~CEffectManager();		// デストラクタ
		HRESULT Init();		// 初期化
		void Uninit();		// 終了
		void Update();		// 更新
		void Draw();		// 描画

		/// <summary>
		/// エフェクシアマネージャー取得
		/// </summary>
		/// <returns>エフェクシアマネージャーのポインター</returns>
		Effekseer::ManagerRef GetManager() { return m_managerRef; }
		/// <summary>
		/// エフェクシアレンダーの取得
		/// </summary>
		/// <returns>エフェクシアレンダーのポインター</returns>
		EffekseerRenderer::RendererRef GetRenderer() { return m_rendererRef; }

	private:
		Effekseer::ManagerRef m_managerRef;	// エフェクシアのエフェクトマネージャー
		EffekseerRenderer::RendererRef m_rendererRef;	// エフェクシアのレンダラー
	};


}

#endif // !_EFFECT_MANAGER_H_