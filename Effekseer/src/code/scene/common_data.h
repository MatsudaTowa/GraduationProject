//===========================================
// 
// シーン間データ保持[common_data.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_
//#include "../system/pch.h"	//プリコンパイルヘッダ
#include "../object/object.h"	// オブジェクト
namespace My
{
	namespace Scene {
		class CCommonData
		{
		public:
			CCommonData() {}
			virtual ~CCommonData() {}
		private:
		};
	}
}


#endif // !_COMMON_DATA_H_