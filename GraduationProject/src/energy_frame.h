//============================================================================================================
//
// エナジーUI枠 「energy_frame.h」
// Author : 笹生一颯
//
//============================================================================================================

#ifndef _ENERGY_FRAME_H_
#define _ENERGY_FRAME_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class CEnergy_frame : public CObject2D
	{
	public:
		/**
		* @brief コンストラクタ
		* @param [in]プライオリティ
		*/
		CEnergy_frame(int nPriority);

		/** 
		* @brief デストラクタ
		*/
		~CEnergy_frame()override;

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
		* @return エナジーUI枠のポインタ
		*/
		static CEnergy_frame* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	};
}

#endif // !_ENERGY_FRAME_H_
