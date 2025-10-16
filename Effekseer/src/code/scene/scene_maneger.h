//===========================================
// 
// シーン[scene.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _SECENE_MANEGER_H_
#define _SECENE_MANEGER_H_

#include "scene_base.h"
#include "../scene/common_data.h"	// シーン間データ保持
namespace My
{
	namespace Scene {
		class CCommonData;
		class CSceneManager : public CBase
		{
		public:
			CSceneManager();
			CSceneManager(const CSceneManager&) = delete;
			CSceneManager& operator = (const CSceneManager&) = delete;
			virtual ~CSceneManager();

			CBase* Update() override;
			void Draw() const override;
			bool GetPose()override;
		private:
			CBase* m_pScene;	// シーン
		};

	}

}

#endif // !_SECENE_MANEGER_H_