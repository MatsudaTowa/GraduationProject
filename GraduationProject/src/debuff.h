//=============================================
//
//デバフ[debuff.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DEBUFF_H_
#define _DEBUFF_H_

#include "character.h"
#include "active_scene_character_state.h"
#include "death_icon.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief デバフクラス */
	class CDebuff
	{
	public:
		enum DEBUFF_TYPE
		{
			DEATH,
			MAX
		};
		/**
		 * @brief コンストラクタ
		 */
		CDebuff() {};
		/**
		 * @brief デストラクタ
		 */
		virtual ~CDebuff() {};
		/**
		* @brief デバフ処理
		* @param [in]キャラクターポインタ
		*/
		virtual void Debuff([[maybe_unused]] CActiveSceneCharacter* character) {};

		/**
		 * @brief タイプ取得
		 * @return タイプ
		 */
		inline DEBUFF_TYPE GetDebuffType()
		{
			return m_type;
		}

		/**
		 * @brief タイプの設定
		 * @param [in]type
		 */
		inline void SetType(DEBUFF_TYPE type)
		{
			m_type = type;
		}
	private:
		DEBUFF_TYPE m_type;
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
		void Debuff(CActiveSceneCharacter* character)override;
	private:
		CDeathIcon* m_pDeathIcon;
		
	};
}
#endif // !_DEBUFF_H_
