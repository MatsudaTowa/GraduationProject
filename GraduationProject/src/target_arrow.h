//=============================================
//
// ターゲットアロー[target_arrow.h]
// Author : 梅田爽真
//
//=============================================
#ifndef _TARGET_ARROW_ //これが定義されてないとき
#define _TARGET_ARROW_
#include "main.h"
#include "object2D_anim.h"

namespace
{
	const D3DXVECTOR2 arrow_up = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.2f };
	const D3DXVECTOR2 arrow_down = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.8f };
	const D3DXVECTOR2 arrow_right = { SCREEN_WIDTH * 0.7f,SCREEN_HEIGHT * 0.5f };
	const D3DXVECTOR2 arrow_left = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.5f };
}

/** @brief My 名前空間 */
namespace My
{
	/** @brief テクスチャ管理クラス */
	class CTargetArrow:public CObject2D_Anim
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CTargetArrow(int nPriority = 5);

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
		* @brief 生成
		*/
		static CTargetArrow* Create(int attacker, int target);

		/**
		* @brief 標的の位置を設定する
		*/
		static D3DXVECTOR2 SetTargetPos(D3DXVECTOR2 &target,int targetnum);

		/**
		* @brief 相手と自分の間の線上に位置を設定する
		*/
		void SetOnTheLinePos();

		inline D3DXVECTOR2 GetAttacker() { return m_attacker; }
		inline D3DXVECTOR2 GetTarget() { return m_target; }

	private:

		/**
		* @brief 最大サイズ
		*/
		const float MAX_SIZE = 250.0f;

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

		/**
		* @brief 矢印のベースサイズ
		*/
		D3DXVECTOR2 m_basesize;

		/**
		* @brief 矢印の三角の部分のポインタ
		*/
		CObject2D* m_triangle;
	};
}

#endif