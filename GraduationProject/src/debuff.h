//=============================================
//
//デバフ[debuff.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DEBUFF_H_
#define _DEBUFF_H_

#include "condition.h"
#include "active_scene_character_state.h"
#include "death_icon.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief デバフクラス */
	class CDebuff:public CCondition
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CDebuff() {};
		/**
		 * @brief デストラクタ
		 */
		~CDebuff()override {};
		/**
		* @brief デバフ処理
		* @param [in]キャラクターポインタ
		*/
		void Condition([[maybe_unused]] CActiveSceneCharacter* character)override {};
	private:
	};

	/** @brief 死亡デバフ */
	class CDeath:public CDebuff
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CDeath();
		/**
		 * @brief デストラクタ
		 */
		~CDeath() override;
		/**
		* @brief 死亡処理
		* @param [in]キャラクターポインタ
		*/
		void Condition(CActiveSceneCharacter* character)override;
	private:
		CDeathIcon* m_pDeathIcon;
		
	};
}
#endif // !_DEBUFF_H_
