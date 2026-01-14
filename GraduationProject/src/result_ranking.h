//=============================================
//
// リザルト用ランキングポリゴン[result_ranking.h]
// Author Ueda Kou
//
//=============================================
#ifndef _RESULT_RANKING_H_
#define _RESULT_RANKING_H_
#include "object2D.h"	// 2Dオブジェクト

// 直接CObject2Dを生成出来ないから作成
namespace My {
	class CResultRanking: public CObject2D
	{
	public:
		CResultRanking();
		~CResultRanking();
		HRESULT Init();		// 初期化
		void SetPos(D3DXVECTOR3 pos)	{ CObject2D::SetPos (pos); }
		void SetRot(D3DXVECTOR3 rot)	{ CObject2D::SetRot (rot); }
		void SetSize(D3DXVECTOR2 size)	{ CObject2D::SetSize(size); }

		static CResultRanking* Create();	// 生成
	private:
	};
}

#endif // !_RESULT_RANKING_H_