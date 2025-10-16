//===========================================
// 
// タイトルシーン[scene_title.cpp]
// Auther:UedaKou
// 
//===========================================
#include "scene_effekseer.h"	// タイトルシーン
#include "../system/manager.h"	// 全体マネージャー
#include "../object/effect/Effekseer.h"

namespace My
{
	namespace Scene {

		CSene_Effekseer::CSene_Effekseer(CBase* scene) :
			CBase(*scene)
		{
			firld = CObject3D::create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
			firld->SetColor(D3DXCOLOR(0.0f, 0.5f, 0.2f, 1.0f));
			Effect::create("");
		}

		CSene_Effekseer::~CSene_Effekseer()
		{
		}

		CBase* CSene_Effekseer::Update()
		{


			return this;
		}

		void CSene_Effekseer::Draw() const
		{
		}

		bool CSene_Effekseer::GetPose()
		{
			return false;
		}

		template<>
		Scene::CBase* CBase::makeScene<CSene_Effekseer>() {
			return new CSene_Effekseer(this);
		}
	}
}