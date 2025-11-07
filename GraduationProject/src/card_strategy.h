//===============================================================================
//
//カードストラテジー[active_scene_character_state.h]
//Author Matsuda Towa
//
//===============================================================================
#ifndef _CARD_STRATEGY_H_
#define _CARD_STRATEGY_H_

/** @brief My 名前空間 */
namespace My
{
	class CActiveSceneCharacter;
	class CCard;
	/** @brief カードストラテジーの基底クラス */
	class CCardStrategy_Base
	{
	public:
		CCardStrategy_Base() {};
		virtual ~CCardStrategy_Base() {};
		/**
		* @brief カード処理(親では何もしない)
		* @param [in]キャラクターポインタ
		* @param [in]カードポインタ
		*/
		virtual void Strategy([[maybe_unused]] CActiveSceneCharacter* character, [[maybe_unused]] CCard* card) {};
	};

	/** @brief カードストラテジーの攻撃クラス */
	class CAttack :public CCardStrategy_Base
	{
	public:
		CAttack();
		~CAttack() override;
		/**
		* @brief 攻撃処理
		* @param [in]キャラクターポインタ
		* @param [in]カードポインタ
		*/
		void Strategy(CActiveSceneCharacter* character, CCard* card) override;
	};

	/** @brief カードストラテジーの守備クラス */
	class CDiffence :public CCardStrategy_Base
	{
	public:
		CDiffence();
		~CDiffence() override;
		/**
		* @brief 防御処理
		* @param [in]キャラクターポインタ
		* @param [in]カードポインタ
		*/
		void Strategy(CActiveSceneCharacter* character, CCard* card) override;
		void Strategy(CActiveSceneCharacter* character, CCard* card, CCard* attack_card) {};
	};

	/** @brief カードストラテジー体力回復クラス */
	class CHeal :public CCardStrategy_Base
	{
	public:
		CHeal();
		~CHeal() override;
		/**
		* @brief 回復処理
		* @param [in]キャラクターポインタ
		* @param [in]カードポインタ
		*/
		void Strategy(CActiveSceneCharacter* character, CCard* card) override;
	};

	/** @brief カードストラテジーエナジーの変動クラス */
	class CEnergyAdjust :public CCardStrategy_Base
	{
	public:
		CEnergyAdjust();
		~CEnergyAdjust() override;
		/**
		* @brief エナジー変動処理
		* @param [in]キャラクターポインタ
		* @param [in]カードポインタ
		*/
		void Strategy(CActiveSceneCharacter* character, CCard* card) override;
	};
}
#endif // !_CARD_STRATEGY_H_
