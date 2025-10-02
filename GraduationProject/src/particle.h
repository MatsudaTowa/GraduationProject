//=============================================
//
//パーティクル[particle.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PARTICLE_H_ //これが定義されてないとき

#define _PARTICLE_H_
#include "main.h"
#include "billboard.h"
#include "count.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief マウスの軌跡クラス
	 * @copydoc CBillboard_Anim
	 */
	class CParticle : public CBillboard_Anim
	{
	public:
		static const int PRIORITY = 21;		//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CParticle(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CParticle()override;

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
		 * @brief 移動量取得
		 * @return 移動量
		 */
		inline D3DXVECTOR3 GetMove()
		{
			return m_move;
		}

		/**
		 * @brief 体力取得
		 * @return 体力
		 */
		inline int GetLife()
		{
			return m_life;
		}

		/**
		 * @brief 最初の色取得
		 * @return 最初の色
		 */
		inline D3DXCOLOR GetStartColor()
		{
			return m_startcolor;
		}

		/**
		 * @brief 移動量設定
		 * @param [in]移動量
		 */
		inline void SetMove(D3DXVECTOR3 move)
		{
			m_move = move;
		}

		/**
		 * @brief 体力設定
		 * @param [in]体力
		 */
		inline void SetLife(int life)
		{
			m_life = life;
		}

		/**
		 * @brief 最初の体力設定
		 * @param [in]最初の体力
		 */
		inline void SetStartLife(int startlife)
		{
			m_startlife = startlife;
		}

		/**
		 * @brief 最初の色設定
		 * @param [in]最初の色
		 */
		inline void SetStartColor(D3DXCOLOR color)
		{
			m_startcolor = color;
		}
	private:
		D3DXVECTOR3 m_move;
		int m_life;
		int m_startlife;
		D3DXCOLOR m_startcolor;
	};
}

#endif