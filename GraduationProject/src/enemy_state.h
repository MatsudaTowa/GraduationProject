//=============================================
//
//エネミーのステートパターン[enemy_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

#include "character.h"
#include "active_scene_character_state.h"
#include "waitchecker_UI.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CEnemy;

	/** @brief エネミーのロビークラス */
	class CEnemyLobbyState :public CLobbyCharacter
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CEnemyLobbyState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CEnemyLobbyState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief エネミーのデュエルクラス */
	class CEnemyDuelState :public CDuelCharacter
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CEnemyDuelState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CEnemyDuelState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Duel(CActiveSceneCharacter* character) override;

		/**
		 * @brief 待機ゾーンの表示処理
		 * @param [in]zone
		 */
		void ViewWait(CWaitZone* zone) override;
	private:
		/**
		 * @brief UI生成
		 * @param [in]enemy
		 */
		void CreateDuelUI(CEnemy* enemy);

		//変数
		std::vector<My::CWaitChecker_UI*> m_WaitCheckerVector;
	};
}
#endif // !_ENEMY_STATE_H_
