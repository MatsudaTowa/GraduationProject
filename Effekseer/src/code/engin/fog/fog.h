//===========================================
// 
// フォグ[fog.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _FOG_H_
#define _FOG_H_

namespace My
{

	class CFog
	{
	public:
		typedef enum {
			EXP = 0,	// 密度
			LINEAR,		// 範囲
			MAX,
		}FogType;	//	フォグ種類
		CFog();
		~CFog();

		void SetFogType(FogType type);		// 種類設定
		void SetFogLinear(float fStart, float fEnd);	// 距離設定
		void SetFogDensity(float fDensity);				// 密度設定

		static CFog* create();	// 生成

	private:
		FogType m_type;			// 種類

		float m_fFogStartPos;	// フォグ開始位置
		float m_fFogEndPos;		// フォグ終了位置

		float m_m_fFogDensity;	// フォグ密度
	};
}
#endif // !_FOG_H_