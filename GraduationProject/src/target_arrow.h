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

/** @brief My 名前空間 */
namespace My
{
	namespace
	{
		const D3DXVECTOR2 arrow_up = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.3f };
		const D3DXVECTOR2 arrow_down = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.7f };
		const D3DXVECTOR2 arrow_right = { SCREEN_WIDTH * 0.7f,SCREEN_HEIGHT * 0.5f };
		const D3DXVECTOR2 arrow_left = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.5f };
	}

	/** @brief テクスチャ管理クラス */
	class CTargetArrow:public CObject2D_Anim
	{
	public:
		/**
		* @brief アローのターゲットする対象
		* (TODO : 敵と自分の矢印を判別する際に使う)
		* @param 自分
		* @param 相手
		*/
		enum ARROWTYPE
		{
			ARROWTYPE_PLAYER = 0,
			ARROWTYPE_ENEMY
		};

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
		D3DXVECTOR2 SetTargetPos(int targetnum,int type);

		/**
		* @brief 相手と自分の間の線上に位置を設定する
		*/
		void SetOnTheLinePos();

		/**
		* @brief 攻撃者取得
		*/
		inline D3DXVECTOR2 GetAttacker() { return m_attacker; }

		/**
		* @brief 攻撃対象者取得
		*/
		inline D3DXVECTOR2 GetTarget() { return m_target; }

		/**
		* @brief 寿命設定
		* @param [in]寿命
		*/
		inline void SetLife(int life) { m_nLife = life; };

		/**
		* @brief 寿命設定
		* @param [out]寿命
		*/
		inline int GetLife() { return m_nLife; };

		/**
		* @brief 寿命リセット
		* @param [in]寿命
		*/
		inline void ResetLife() { m_nLife = ONE_SECOND * 3; };

		/**
		* @brief 矢印の種類を設定する
		* @param [in]種類
		*/
		inline void SetArrowType(ARROWTYPE type) { m_ArrowType = type; };

		/**
		* @brief 矢印の種類を取得する
		* @param [out]種類
		*/
		inline ARROWTYPE GetArrowType() { return m_ArrowType; }

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

		/**
		* @brief 矢印のベースサイズ
		*/
		D3DXVECTOR2 m_basesize;

		/**
		* @brief 矢印の三角の部分のポインタ
		*/
		CObject2D* m_triangle;

		/**
		* @brief 寿命(矢印自体の寿命)
		*/
		int m_nLife;

		/**
		* @brief 矢印のタイプ
		*/
		ARROWTYPE m_ArrowType;
	};
}

#endif