//============================================
//
// ŒõŒ¹[light.h]
// Author:Uedakou
// 
//============================================
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "../../system/main.h"	

namespace My
{

#define MAX_LIGHT (3)	// ŒõŒ¹¶¬”
	class CLight
	{
	public:
		CLight();
		~CLight();
		HRESULT Init();	// ‰Šú‰»
		void Uninit();	// I—¹
		void Update();	// XV
	private:
		D3DLIGHT9 m_light[MAX_LIGHT];	// ŒõŒ¹
	};

}
#endif // _LIGHT_H_