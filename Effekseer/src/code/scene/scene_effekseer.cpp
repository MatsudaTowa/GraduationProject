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
			// フィールド
			firld = CObject3D::create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
			firld->SetColor(D3DXCOLOR(0.0f, 0.5f, 0.2f, 1.0f));

			// エフェクト
			m_pEffect = Effect::create("data/Flame.efk");
			m_pEffect->SetLoop(true);
		}

		CSene_Effekseer::~CSene_Effekseer()
		{
		}

		CBase* CSene_Effekseer::Update()
		{
			CManager* pManager = CManager::GetInstance();
			CInputKeyboard* pKye = pManager->GetInKey();
			// 移動
			if (pKye->GetTrigger(DIK_W))
			{
				m_pEffect->AddPos({ 0.0f, 0.0f, m_move });
			}
			else if (pKye->GetTrigger(DIK_A))
			{
				m_pEffect->AddPos({ -m_move, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_S))
			{
				m_pEffect->AddPos({ 0.0f, 0.0f, -m_move });
			}
			else if (pKye->GetTrigger(DIK_D))
			{
				m_pEffect->AddPos({ m_move, 0.0f, 0.0f });
			}
			// 変形
			if (pKye->GetTrigger(DIK_NUMPAD1))
			{
				m_pEffect->AddScl({ 0.0f, 0.0f, m_scl });
			}
			else if (pKye->GetTrigger(DIK_NUMPAD4))
			{
				m_pEffect->AddScl({ 0.0f, 0.0f, -m_scl });
			}
			else if (pKye->GetTrigger(DIK_NUMPAD2))
			{
				m_pEffect->AddScl({ m_scl, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_NUMPAD5))
			{
				m_pEffect->AddScl({ -m_scl, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_NUMPAD3))
			{
				m_pEffect->AddScl({ 0.0f, m_scl, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_NUMPAD6))
			{
				m_pEffect->AddScl({ 0.0f, -m_scl, 0.0f });
			}
			// 回転
			if (pKye->GetTrigger(DIK_R))
			{
				m_pEffect->AddRot({ 0.0f, 0.0f, m_rol });
			}
			else if (pKye->GetTrigger(DIK_F))
			{
				m_pEffect->AddRot({ 0.0f, 0.0f, -m_rol });
			}
			else if (pKye->GetTrigger(DIK_T))
			{
				m_pEffect->AddRot({ m_rol, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_G))
			{
				m_pEffect->AddRot({ -m_rol, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_Y))
			{
				m_pEffect->AddRot({ 0.0f, m_rol, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_H))
			{
				m_pEffect->AddRot({ 0.0f, -m_rol, 0.0f });
			}

			// 色
			if (pKye->GetTrigger(DIK_U))
			{
				m_pEffect->AddCol({ m_col, 0.0f, 0.0f , 0.0f});
			}
			else if (pKye->GetTrigger(DIK_J))
			{
				m_pEffect->AddCol({ -m_col, 0.0f, 0.0f , 0.0f });
			}
			else if (pKye->GetTrigger(DIK_I))
			{
				m_pEffect->AddCol({ 0.0f, m_col, 0.0f , 0.0f });
			}
			else if (pKye->GetTrigger(DIK_K))
			{
				m_pEffect->AddCol({ 0.0f, -m_col, 0.0f , 0.0f });
			}
			else if (pKye->GetTrigger(DIK_O))
			{
				m_pEffect->AddCol({ 0.0f, 0.0f, m_col , 0.0f });
			}
			else if (pKye->GetTrigger(DIK_L))
			{
				m_pEffect->AddCol({ 0.0f, 0.0f, -m_col , 0.0f });
			}
			else if (pKye->GetTrigger(DIK_P))
			{
				m_pEffect->AddCol({ 0.0f, 0.0f, 0.0f , m_col });
			}
			else if (pKye->GetTrigger(DIK_SEMICOLON))
			{
				m_pEffect->AddCol({ 0.0f, 0.0f, 0.0f , -m_col });
			}

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