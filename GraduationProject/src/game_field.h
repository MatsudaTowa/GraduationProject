//=============================================
//
//ゲームの床[game_field.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_FIELD_H_ //これが定義されてないとき

#define _GAME_FIELD_H_
#include "main.h"
#include "field.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief ゲーム床クラス
	 * @copydoc CField
	 */
	class CGameField : public CField
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CGameField(int nPriority = FIELD_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CGameField()override;

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
	private:
		static const std::string TEXTURE_NAME;	//!<テクスチャの名前
	};
}

#endif