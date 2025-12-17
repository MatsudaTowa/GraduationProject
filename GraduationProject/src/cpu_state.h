//=============================================
//
//コンピューターのステート[cpu_state.h]
//Author 島津 湧人
//
//=============================================
#ifndef _CPU_STATE_H_
#define _CPU_STATE_H_

#include "enemy_state.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CEnemy;

	/** @brief コンピューターのロビークラス */
	class CCpuLobbyState :public CEnemyLobbyState
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCpuLobbyState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CCpuLobbyState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief コンピューターのデュエルクラス */
	class CCpuDuelState :public CEnemyDuelState
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCpuDuelState(CActiveSceneCharacter* character);
		/**
		 * @brief デストラクタ
		 */
		~CCpuDuelState() override;
		/**
		* @brief デュエル処理
		* @param [in]キャラクターポインタ
		*/
		void Duel(CActiveSceneCharacter* character) override;
	private:

	};
}
#endif // !_ENEMY_STATE_H_
