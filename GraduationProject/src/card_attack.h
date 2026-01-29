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
	class CTotalDamageUI;

	class CCardAttack :public CCard
	{
	public:
		enum AttackType
		{// 攻撃の種類
			NONE_ATTACK,            // 攻撃以外のときはこれ
			ALL_ATTACK,                // 全体攻撃
			SPECIFIC_ATTACK,        // 特定の相手を選んで攻撃
			RANDOM_ATTACK,            // ランダム攻撃
			SELFINTARGET_ATTACK,    // 自分を含めた攻撃
		};

		/**
		 * @brief コンストラクタ
		 */
		CCardAttack(int nPriority);

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
		 * @brief 描画
		 */
		void Draw()override;

		void LoadCardData() override;

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
		 * @brief キャストしたかの確認
		 */
		bool IsCast(CDuelCharacter* duel, CInputMouse::AREA area) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuelCharacter* duel) override;

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

		/**
		 * @brief 重ねるカードの追加
		 * @param [in]重ねるカード
		 */
		inline void AddStackCards(CCardAttack* card) 
		{ 
			m_StackedCardsList.push_back(card);
			GetState()->Stack();
		}

		/**
		 * @brief 重ねたカードの取得
		 * @preturn 重なったカードのリスト
		 */
		std::list<CCardAttack*> GetStackedCards() { return m_StackedCardsList; }

		/**
		 * @brief 一番上のキャストカードかのフラグの設定
		 * @param フラグの設定
		 */
		void SetTopCastCard(bool top) { m_isTopCastCard = top; }

		/**
		 * @brief 一番上のキャストカードかのフラグの取得
		 * @param フラグの設定
		 */
		bool GetTopCastCard() { return m_isTopCastCard; }

		/**
		 * @brief ダメージ情報の追加
		 * @param 対象の番号
		 */
		void AddDamageInfo(int id) { m_DamageInfo[id] = m_nAttackValue; }

		/**
		 * @brief ダメージ情報の取得
		 * @return ダメージ情報
		 */
		std::map<int, int>& GetDamageInfo() { return m_DamageInfo; }

		//トリガーを受信した際の処理
		void ReceiveTrigger() override;

		//カード情報の送信
		void SendCardInfo(RakNet::BitStream* bsout) override;

		//ターゲットリストの追加
		void AddTargetPlayerList(CActiveSceneCharacter* character) override;

		//カード情報の読み込み
		void LoadCardInfo(RakNet::BitStream* bsin) override;

		//ステイ中に並べるカードか
		bool IsSetUpStay() override;

		//ステイ時間を表示するか
		bool IsCreatStayCount() override { return m_isTopCastCard; }

		CTotalDamageUI* GetTotalDamageUI() { return m_pTotalDamageUI; }
	private:

		AttackType m_AttackType;						// 攻撃の種類
		int m_nAttackValue;								// ダメージ数
		std::map<int, int> m_DamageInfo;				// ダメージ情報
		std::vector<CCardDeffence*> m_DefCardVector;	// 守備カードのベクター
		std::list<CCardAttack*> m_StackedCardsList;		// 重なっているカード
		bool m_isTopCastCard;							// 一番上のカードか
		CCardAttack* m_pStackCard;						// 重ねたカード
		CTotalDamageUI* m_pTotalDamageUI;				// トータルダメージUI
	};
}

#endif // !_CARD_ATK_H_