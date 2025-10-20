//===========================================
// 
// タイトルシーン[scene_title.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _SENE_TITLE_H_
#define _SENE_TITLE_H_
#include "scene_base.h"		// シーンベース
#include "../object/3D/object_3D.h"	// ３Dオブジェクト
#include "../object/effect/Effekseer.h"

namespace My
{
	namespace Scene {
		class CSene_Effekseer : public CBase
		{
		public:
			CSene_Effekseer(CBase* scene);
			~CSene_Effekseer();

		private:
			virtual CBase* Update()	override;
			virtual void Draw() const override;
			bool GetPose()override;
			CObject3D* firld;
			My::Effect* m_pEffect;

			const float m_move = 10.0f;
			const float m_rol = 1.0f;
			const float m_scl = 0.1f;
			const float m_col = 0.1f;
	
		};


	}
}
#endif // !_SENE_TITLE_H_