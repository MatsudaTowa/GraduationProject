//=============================================
//
//ゾーンUI処理「zone_num_UI.h」
// Author松田永久
//
//=============================================
#ifndef _ZONE_NUM_UI_H_ //これが定義されてないとき
#define _ZONE_NUM_UI_H_
#include "main.h"
#include "object2D.h"
#include "zone_num.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 体力クラス */
	class CZoneNumUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CZoneNumUI();
		/**
		 * @brief デストラクタ
		 */
		virtual ~CZoneNumUI();
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		virtual HRESULT Init();
		/**
		 * @brief 終了
		 */
		virtual void Uninit();

		/**
		 * @brief 数字の設定
		 * @param [in]枚数
		 */
		void SetNumber(int num);

		/**
		 * @brief 数字の座標設定
		 * @param [in]pos
		 */
		inline void SetNumPos(D3DXVECTOR3 pos)
		{
			m_num_pos = pos;
		}

		inline D3DXVECTOR3 GetNumPos()
		{
			return m_num_pos;
		}

		inline std::vector<CZoneNum*> GetNumVector()
		{
			return  m_pNumber;
		}

	private:
		/**
		 * @brief 桁追加
		 * @param [in]U座標
		 * @param [in]桁数
		 * @param [in]体力
		 */
		void AddDigit(std::vector<int>& pos_texU, int& num_digit, int num);

		/**
		 * @brief 桁削除
		 * @param [in]テクスチャU座標
		 * @param [in]桁数
		 */
		void EraseDigit(std::vector<int> pos_texU, int& num_digit);

		D3DXVECTOR3 m_num_pos;					//数字の位置
		std::vector<CZoneNum*> m_pNumber;		//数字
	};
}

#endif // DEBUG