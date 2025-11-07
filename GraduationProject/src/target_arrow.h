//=============================================
//
// ターゲットアロー[target_arrow.h]
// Author : 梅田爽真
//
//=============================================
#ifndef _TARGET_ARROW_ //これが定義されてないとき
#define _TARGET_ARROW_
#include "main.h"

namespace
{
	const D3DXVECTOR2 up = { SCREEN_WIDTH * 0.5f,0.0f };
	const D3DXVECTOR2 down = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT};
	const D3DXVECTOR2 right = {SCREEN_WIDTH*0.8f,SCREEN_HEIGHT * 0.5f };
	const D3DXVECTOR2 left = { SCREEN_WIDTH * 0.2f,SCREEN_HEIGHT * 0.5f };
}

/** @brief My 名前空間 */
namespace My
{
	/** @brief テクスチャ管理クラス */
	class CTargetArrow:public CObject2D
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CTargetArrow(int nPriority = 10);

		/**
		* @brief デストラクタ
		*/
		~CTargetArrow()override;

		/**
		* @brief 初期化
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
		* @brief 
		*/
		static CTargetArrow* Create(int attacker, int target);

		static D3DXVECTOR2 SetTargetPos(D3DXVECTOR2 &target,int targetnum);
	private:
		/**
		* @brief 角度と長さを設定
		*/
		void SetAngleLength();

		/**
		* @brief 攻撃対象者
		*/
		D3DXVECTOR2 m_target;

		/**
		* @brief 攻撃者
		*/
		D3DXVECTOR2 m_attacker;
	};
}

#endif