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
		 * @brief ディフェンスの値取得
		 * @return ディフェンスの値
		 */
		inline int GetDefenceValue() { return m_nDefenceValue; }

		/**
		 * @brief カウンターの値取得
		 * @return カウンターの値
		 */
		inline int GetCounterValue() { return m_nCounterValue; }

		/**
		 * @brief 守備のタイプ設定
		 * @param [in]attack_type
		 */
		inline void SetDefenceType(DefenseType defence_type) { m_DefenceType = defence_type; }

		/**
		 * @brief ディフェンスの値取得
		 * @param ディフェンスの値
		 */
		inline void SetDefenceValue(int defence_value) { m_nDefenceValue = defence_value; }

		/**
		 * @brief カウンターの値設定
		 * @param カウンターの値
		 */
		inline void SetCounterValue(int counter_value) { m_nCounterValue = counter_value; }

		/**
		 * @brief 固有情報設定
		 * @param [in]param
		 */
		void LoadUniqueInfo(CCard_Client::Param param) override;

		/**
		 * @brief キャストしたかの確認
		 */
		bool IsCast(CDuelCharacter* duel) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuelCharacter* duel) override;

		/**
		 * @brief トリガー処理
		 */
		void Trigger() override;

	private:

		DefenseType m_DefenceType;
		int m_nDefenceValue;
		int m_nCounterValue;
	};
}

#endif // !_CARD_DF_H_