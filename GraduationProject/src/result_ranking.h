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
		void AddPos(D3DXVECTOR3 pos)	{ SetPos(GetPos() + pos); }		// 位置加算
		void AddRot(D3DXVECTOR3 rot)	{ SetRot(GetRot() + rot); }		// 向き加算
		void AddSize(D3DXVECTOR2 size)	{ SetSize(GetSize() + size); }	// 大きさ加算
		void AddColor(D3DXCOLOR col)	{
			D3DXCOLOR color = GetColor();
			D3DXCOLOR colorA = color + col;
			SetColor (colorA);
		}	// 色の加算
		static CResultRanking* Create();	// 生成
	private:
	};
}

#endif // !_RESULT_RANKING_H_