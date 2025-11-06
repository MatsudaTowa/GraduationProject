//=============================================
//
//デバフ[condi.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "character.h"
#include "active_scene_character_state.h"
#include "death_icon.h"

/** @brief My 名前空間 */
namespace My
{
	class CCondition
	{
	public:
		enum CONDITION_TYPE
		{
			DEATH,
			MAX
		};
		/**
		 * @brief コンストラクタ
		 */
		CCondition() {};
		/**
		 * @brief デストラクタ
		 */
		virtual ~CCondition() {};
		/**
		* @brief デバフ処理
		* @param [in]キャラクターポインタ
		*/
		virtual void Condition([[maybe_unused]] CActiveSceneCharacter* character) {};

		/**
		 * @brief タイプ取得
		 * @return タイプ
		 */
		inline CONDITION_TYPE GetConditionType()
		{
			return m_type;
		}

		/**
		 * @brief タイプの設定
		 * @param [in]type
		 */
		inline void SetType(CONDITION_TYPE type)
		{
			m_type = type;
		}
	private:
		CONDITION_TYPE m_type;
	};
}
#endif