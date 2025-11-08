//===========================================================================================================================================================
// 
// ディフェンスカード [card_deffence.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_DEF_H_
#define _CARD_DEF_H_
#include "main.h"
#include "card.h"

namespace My
{
	class CCardDeffence :public CCard
	{
	public:
		enum DefenseType
		{// 守備の種類
			NONE_DEFENSE,        // 守備以外のときはこれ
			COUNTER,            // 反撃できる
			NOT_COUNTER,        // 反撃できない
		};

		/**
		 * @brief コンストラクタ
		 */
		CCardDeffence(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCardDeffence()override;

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

		void LoadCardData() override;

		/**
		 * @brief 守備のタイプ取得
		 * @return m_AttackType
		 */
		inline DefenseType GetDefenceType() { return m_DefenceType; }

		/**
		 * @brief 守備のタイプ設定
		 * @param [in]attack_type
		 */
		inline void SetDefenceType(DefenseType defence_type) { m_DefenceType = defence_type; }
	private:
		DefenseType m_DefenceType;
	};
}

#endif // !_CARD_DF_H_