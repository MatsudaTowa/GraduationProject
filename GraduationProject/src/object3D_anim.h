//=============================================
//
//アニメーションする3Dオブジェクト[object3D_anim.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT_3D_ANIM_H_ //これが定義されてないとき

#define _OBJECT_3D_ANIM_H_
#include "main.h"
#include "object3D.h"
#include "count.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief アニメーション3Dオブジェクトクラス */
	class CObject3D_Anim : public CObject3D
	{
	public:
		CObject3D_Anim(int nPriority);
		~CObject3D_Anim()override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		//アニメーション処理
		void AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move);
		//アニメーションフレーム代入
		inline void SetAnimFrame(int nAnimFrame)
		{
			m_pCnt->SetFrame(nAnimFrame);
		}

		//テクスチャ移動量の代入
		inline void SetTexMove(D3DXVECTOR2 tex_move)
		{
			m_tex_move = tex_move;
		}

		//アニメーションフレーム取得
		inline int GetAnimFrame()
		{
			return m_pCnt->GetFrame();
		}

		//テクスチャ移動量取得
		inline D3DXVECTOR2& GetTexMove()
		{
			return m_tex_move;
		}

	private:
		CCount* m_pCnt;
		D3DXVECTOR2 m_tex_move; //tex移動量

	};
}
#endif