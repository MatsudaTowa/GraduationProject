//=============================================
//
//アニメーションする2Dオブジェクト[object2D_anim.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT_2D_ANIM_H_ //これが定義されてないとき

#define _OBJECT_2D_ANIM_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{

	/** @brief アニメーション2Dクラス */
	class CObject2D_Anim : public CObject2D
	{
	public:
		CObject2D_Anim(int nPriority);
		~CObject2D_Anim()override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		//アニメーション処理
		void AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move);
		//アニメーションフレーム代入
		inline void SetAnimFrame(int nAnimFrame)
		{
			m_nAnimFrame = nAnimFrame;
		}

		//テクスチャ移動量の代入
		inline void SetTexMove(D3DXVECTOR2 tex_move)
		{
			m_tex_move = tex_move;
		}

		//アニメーションフレーム取得
		inline int GetAnimFrame()
		{
			return m_nAnimFrame;
		}

		//テクスチャ移動量取得
		inline D3DXVECTOR2& GetTexMove()
		{
			return m_tex_move;
		}

	private:
		//アニメーション
		int m_nAnimFrame; //アニメーションフレーム
		int m_nAnimCnt; //アニメーションカウント
		D3DXVECTOR2 m_tex_move; //tex移動量

	};
}
#endif