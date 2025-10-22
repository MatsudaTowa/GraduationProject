//=============================================
//
//準備できているかのテキスト表記[ready_txt.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _READY_TXT_H_ //これが定義されてないとき

#define _READY_TXT_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//準備できてるかの文字クラス TODO:将来的にフォントに変更
	//=============================================
	class CReadyTxt : public CObject2D
	{
	public:
		static const int PRIORITY = 25;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CReadyTxt(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CReadyTxt()override;
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
		 * @param [in]色
		 * @return
		 */
		static CReadyTxt* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	private:
	};
}

#endif