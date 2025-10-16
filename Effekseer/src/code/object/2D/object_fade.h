//===========================================
// 
// フェード[object_fade.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _FADE_H_
#define _FADE_H_

#include "../2D/object_2D.h"	// オブジェクト2D

namespace My
{

	class CFade : public CObject2D
	{
	public:
		enum class TYPE {
			WHITE_OUT = 0,	// 白塗り
			WHITE_IN,
			BLACK_OUT,
			BLACK_IN,
			MAX
		};
		CFade();
		CFade(int nPriority);	// 優先度付き
		virtual ~CFade()override;
		virtual HRESULT Init()		override;	// 初期化
		virtual void Uninit()	override;	// 終了
		virtual void Update()	override;	// 更新
		virtual void Draw()		override;	// 描画

		static CFade* create(TYPE type, int nCnt);
		static CFade* create(int nPriority, TYPE type, int nCnt);
	private:
		int m_nCnt;
		int m_nCntMax;
		TYPE m_type;
	};
}
#endif // !_FADE_H_