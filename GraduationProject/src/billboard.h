//=============================================
//
//ビルボード[billboard.h]
//Auther Matsuda Towa
//
//=============================================
#ifndef _BILLBOARD_H_ //これが定義されてないとき

#define _BILLBOARD_H_
#include "main.h"
#include "object3D.h"
#include "object3D_Anim.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief ビルボードクラス */
	class CBillboard : public CObject3D
	{
	public:

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CBillboard(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CBillboard()override;

		/**
		 * @brief 描画
		 */
		void Draw()override;
	};

	/** @brief アニメーションビルボードクラス */
	class CBillboard_Anim : public CObject3D_Anim
	{
	public:

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CBillboard_Anim(int nPriority):CObject3D_Anim(nPriority) {};

		/**
		 * @brief デストラクタ
		 */
		~CBillboard_Anim()override {};

		/**
		 * @brief 描画
		 */
		void Draw()override;
	};
}
#endif