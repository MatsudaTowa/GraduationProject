//=============================================
//
//エナジーUI処理「energy_UI.h」
// Author松田永久
//
//=============================================
#ifndef _ENERGY_UI_H_ //これが定義されてないとき
#define _ENERGY_UI_H_
#include "main.h"
#include "object2D.h"
#include "number.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief エナジーUIクラス */
	class CEnergy_UI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CEnergy_UI();
		/**
		 * @brief デストラクタ
		 */
		~CEnergy_UI();
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init();
		/**
		 * @brief 終了
		 */
		void Uninit();

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @return エナジーUIポインタ
		 */
		static CEnergy_UI* Create(D3DXVECTOR3 pos);

		/**
		 * @brief 数字の設定
		 * @param [in]ライフ数
		 */
		void SetEnergyNumber(int energy);

		/**
		 * @brief 数字の座標設定
		 * @param [in]pos
		 */
		inline void SetNumPos(D3DXVECTOR3 pos)
		{
			m_num_pos = pos;
		}

	private:
		/**
		 * @brief 桁追加
		 * @param [in]U座標
		 * @param [in]桁数
		 * @param [in]エナジー
		 */
		void AddDigit(std::vector<int>& pos_texU, int& num_digit, int energy);

		/**
		 * @brief 桁削除
		 * @param [in]テクスチャU座標
		 * @param [in]桁数
		 */
		void EraseDigit(std::vector<int> pos_texU, int& num_digit);

		D3DXVECTOR3 m_num_pos;					//数字の位置
		std::vector<CNumber_2D*> m_pNumber;		//数字
	};
}

#endif // DEBUG