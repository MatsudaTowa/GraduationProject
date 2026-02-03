//=============================================
//
//何Pかの表示UI[player_number_UI.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PLAYER_NUMBER_UI_H_ //これが定義されてないとき

#define _PLAYER_NUMBER_UI_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//準備できてるかの文字クラス
	//=============================================
	class CPlayerNumberUI : public CObject2D
	{
	public:
		static const int PRIORITY = 20;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CPlayerNumberUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CPlayerNumberUI()override;
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
		 * @param [in]プレイヤーのID
		 * @return
		 */
		static CPlayerNumberUI* Create(D3DXVECTOR3 pos, int player_id);
	private:
		static constexpr int NUM_PLAYER = 4;
		static constexpr float POLY_SIZE = 25.0f;
		static const std::string TEX_NAME[NUM_PLAYER];
	};
}

#endif