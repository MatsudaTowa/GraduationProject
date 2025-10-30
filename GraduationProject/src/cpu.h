//=============================================
//
//エネミー[cpu.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CPU_H_ //これが定義されてないとき

#define _CPU_H_
#include "enemy.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief エネミークラス
	 * @copydoc CActiveSceneCharacter
	 */
	class CCpu :public CEnemy
	{
	public:
		static constexpr int PRIORITY = 8;		//!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CCpu(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CCpu() override;

		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init() override;

		/**
		 * @brief 終了
		 */
		void Uninit() override;

		/**
		 * @brief 更新
		 */
		void Update() override;

		/**
		 * @brief 描画
		 */
		void Draw() override;

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]方向
		 * @param [in]ID
		 * @return エネミーポインタ
		 */
		static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int id);
	private:
	};
}
#endif