//===========================================================================================================================================================
// 
// アタックカード [card_attack.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_ATK_H_
#define _CARD_ATK_H_
#include "main.h"
#include "card.h"
#include "card_deffence.h"

namespace My
{
	class CCardAttack :public CCard
	{
	public:
		enum AttackType
		{// 攻撃の種類
			NONE_ATTACK,            // 攻撃以外のときはこれ
			ALL_ATTACK,             // 全体攻撃
			SPECIFIC_ATTACK,        // 特定の相手を選んで攻撃
			RANDOM_ATTACK,          // ランダム攻撃
			SELFINTARGET_ATTACK,    // 自分を含めた攻撃
		};
		/**
		 * @brief コンストラクタ
		 */
		CCardAttack();

		/**
		 * @brief デストラクタ
		 */
		~CCardAttack()override;

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
		 * @brief 攻撃のタイプ取得
		 * @return m_AttackType
		 */
		inline AttackType GetAttackType() { return m_AttackType; }

		/**
		 * @brief 攻撃力を取得
		 * @return 攻撃
		 */
		inline int GetAttackValue() { return m_nAttackValue; }

		/**
		 * @brief 攻撃のタイプ設定
		 * @param [in]attack_type
		 */
		inline void SetAttackType(AttackType attack_type) { m_AttackType = attack_type; }

		/**
		 * @brief 攻撃量の値設定
		 * @param [in]attack_value
		 */
		inline void SetAttackValue(int attack_value) { m_nAttackValue = attack_value; }

		/**
		 * @brief 固有情報読み込み
		 * @param [in]param
		 */
		void LoadUniqueInfo(CCard_Client::Param param) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuel_Player* player) override;

		/**
		 * @brief ステイ処理
		 */
		void Stay() override;

		/**
		 * @brief トリガー処理
		 */
		void Trigger() override;

		/**
		 * @brief 守備カードの追加
		 * @param [in]attack_value
		 */
		inline void AddDefCard(CCardDeffence* defcard) { m_DefCardVector.push_back(defcard); }

	private:

		AttackType m_AttackType;						// 攻撃の種類
		int m_nAttackValue;								// ダメージ数
		std::vector<CCardDeffence*> m_DefCardVector;	// 守備カードのベクター
	};
}

#endif // !_CARD_ATK_H_