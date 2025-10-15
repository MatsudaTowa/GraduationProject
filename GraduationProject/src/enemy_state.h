////=============================================
////
////エネミーのステートパターン[enemy_state.h]
////Author Matsuda Towa
////
////=============================================
//#ifndef _ENEMY_STATE_H_
//#define _ENEMY_STATE_H_
//
//#include "character.h"
//#include "active_scene_character_state.h"
//
///** @brief My 名前空間 */
//namespace My
//{
//	//=============================================
//	//前方宣言
//	//=============================================
//	class CEnemy;
//
//	/** @brief エネミーのロビークラス */
//	class CLobbyState :public CActiveSceneCharacterState
//	{
//	public:
//		/**
//		 * @brief コンストラクタ
//		 */
//		CLobbyState();
//		/**
//		 * @brief デストラクタ
//		 */
//		~CLobbyState() override;
//		/**
//		* @brief デュエル処理
//		* @param [in]キャラクターポインタ
//		*/
//		void Lobby(CActiveSceneCharacter* character) override;
//	};
//
//	/** @brief エネミーのデュエルクラス */
//	class CDuelState :public CActiveSceneCharacterState
//	{
//	public:
//		/**
//		 * @brief コンストラクタ
//		 */
//		CDuelState();
//		/**
//		 * @brief デストラクタ
//		 */
//		~CDuelState() override;
//		/**
//		* @brief デュエル処理
//		* @param [in]キャラクターポインタ
//		*/
//		void Duel(CActiveSceneCharacter* character) override;
//	private:
//		/**
//		 * @brief UI生成
//		 * @param [in]enemy
//		 */
//		void CreateDuelUI(CEnemy* enemy);
//	};
//}
//#endif // !_ENEMY_STATE_H_
