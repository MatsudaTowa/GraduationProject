//============================================================================================================
//
// 体力UI枠 「life_frame.h」
// Author : 笹生一颯
//
//============================================================================================================
#ifndef _LIFE_FRAME_H_
#define _LIFE_FRAME_H_

//===================================================
// ヘッダーファイルインクルード
//===================================================
#include "object2D.h"
#include "main.h"

//===================================================
// クラス定義
//===================================================
namespace My
{
	class CLife_frame : public CObject2D
	{
	public:
		/**
		* @brief コンストラクタ
		* @param [in]プライオリティ
		*/
		CLife_frame(int nPriority);

		/**
		* @brief デストラクタ
		*/
		~CLife_frame()override;

		/**
		* @brief 初期化
		* @return 成功したか
		*/
		HRESULT Init()override;

		/**
		* @brief 終了
		*/
		void Uninit()override;

		/**
		* @brief 更新
		*/
		void Update()override;

		/**
		* @brief 描画
		*/
		void Draw()override;

		/**
		* @brief 生成
		* @param [in]位置
		* @param [in]サイズ
		* @return ライフUI枠のポインタ
		*/
		static CLife_frame* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	};
}


#endif // !_LIIFE_FRAME_H_
