//===========================================
// 
// �^�C�g���V�[��[scene_title.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _SENE_TITLE_H_
#define _SENE_TITLE_H_
#include "scene_base.h"		// �V�[���x�[�X
#include "../object/3D/object_3D.h"	// �RD�I�u�W�F�N�g
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
			My::Effect* p;
	
		};


	}
}
#endif // !_SENE_TITLE_H_