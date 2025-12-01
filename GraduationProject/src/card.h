//===========================================================================================================================================================
// 
// カードモデル [card.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_H_
#define _CARD_H_
#include "main.h"
#include "objectX.h"
#include "card_state.h"
#include "card_manager.h"
#include "card_strategy.h"

namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	//class CCard_Client;
	class CActiveScenePlayer;
	class CDuelCharacter;
	class CZone;

	class CCard :public CObject3D
	{
	public:

		/**
		* @brief カードタイプの列挙型
		* @param 攻撃カード
		* @param 防御カード
		* @param 補助カード
		*/
		enum CARDTYPE_
		{
			TYPE_NONE=0,
			TYPE_ATTACK,
			TYPE_DEFFENCE,
			TYPE_ASSIST,
			TYPE_MAX
		};

		enum RARITY
		{// レアリティの種類
			NONE_RARITY,
			COMMON,            // コモン
			UNCOMMON,        // アンコモン
			RARE,            // レア
			SUPERRARE,        // スーパーレア
			HYPERRARE,        // ハイパーレア
			ROYALRARE,        // ロイヤルレア
			EMPERORRARE,    // エンペラーレア
			GODRARE,        // ゴッドレア
			XRARE,            // エックスレア
		};

		enum ZONE
		{//ゾーンの種類
			NONE_ZONE,
			DECK,
			HAND,
			CAST,
			WAIT,
			FIELD,
			CEMETERY
		};

		struct BaseStatus
		{
			int nPackID;						  // パック番号
			int nCardID;						  // カード番号
			std::string name;					  // カード名
			std::string ruby;                     // フリガナ
			int nCost;                            // カードコスト
			CARDTYPE_ maintype;                   // カードの種類
			RARITY raritytype;                    // レアリティの種類
			std::string imagePath;                // 画像ファイルパス
		};

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCard(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCard()override;

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
		void Update(CDuelCharacter* duel);

		/**
		 * @brief 描画
		 */
		void Draw()override;

		/**
		 * @brief キャストしたかの確認
		 */
		virtual bool IsCast(CDuelCharacter* duel) = 0;

		/**
		 * @brief キャスト処理
		 */
		virtual void Cast(CDuelCharacter* duel) {};

		/**
		 * @brief ステイ処理
		 */
		virtual void Stay() = 0;

		/**
		 * @brief トリガー処理
		 */
		virtual void Trigger() = 0;

		/**
		 * @brief カードデータ読み込み
		 */
		virtual void LoadCardData();

		/**
		 * @brief マウスでカードを選択する
		 * @param [out]カードが選択されたかどうかを返す
		 */
		bool CardSelectToMouse(CDuelCharacter* duel);

		/**
		 * @brief カードをマウスでキャストする
		 * @return [out]カードがキャストされたかどうか判定
		 */
		virtual bool CardCastToMouse(CDuelCharacter* duel, CActiveSceneCharacter* player);

		/**
		 * @brief 生成
		 * @param [in]カードタイプ
		 */
		static CCard* Create(CCard::CARDTYPE_ type);

		/**
		* @brief 状態を変更する
		*/
		void ChangeState(CCardState*state);
		void ChangeState(CCardState::CARD_STATE state, CDuelCharacter* duel);

		/**
		* @brief ステート取得
		*/
		inline CCardState* GetState() { return m_pState; }
		inline CCardState::CARD_STATE GetStateNum() { return m_StateNum; }

		/**
		* @brief カードタイプ取得
		*/
		inline void SetCardType(CCard::CARDTYPE_ type) { m_CardType = type; }
		inline CARDTYPE_ GetCardType() { return m_CardType; }

		/**
		* @brief ニュートラル位置
		*/
		inline void SetNeutralPos(D3DXVECTOR3 npos) { m_NeutralPos = npos; }
		inline D3DXVECTOR3 GetNeutralPos() { return m_NeutralPos; }

		/**
		 * @brief ステータス取得
		 * @return m_BaseStatus
		 */
		inline BaseStatus GetBaseStatus() { return m_BaseStatus; }

		/**
		 * @brief ステータス設定
		 * @param [in]status
		 */
		inline void SetBaseStatus(BaseStatus status) { m_BaseStatus = status; }

		/**
		 * @brief 現在のゾーン取得
		 * @return 現在のゾーン
		 */
		inline ZONE GetCurrentZone() { return m_CurrentZone; }

		/**
		 * @brief 過去のゾーン取得
		 * @return 過去のゾーン
		 */
		inline ZONE GetOldZone() { return m_OldZone; }

		/**
		 * @brief 現在のゾーン取得
		 * @param 現在のゾーン
		 */
		inline void SetCurrentZone(ZONE zone) { m_CurrentZone = zone; }

		/**
		 * @brief 過去のゾーン取得
		 * @param 過去のゾーン
		 */
		inline void SetOldZone(ZONE zone) { m_OldZone = zone; }

		/**
		* @brief ニュートラル位置
		*/
		inline void SetTarget(CInputMouse::AREA area) { m_target = area; }
		inline CInputMouse::AREA GetTarget() { return m_target; }

		inline void SetCardHolder(CActiveScenePlayer* player) { m_pCardHolder = player; }
		inline CActiveScenePlayer* GetCardHolder() { return m_pCardHolder; }

		inline void SetUpdate(bool update) { m_isUpdate = update; }
		inline bool GetUpdate() { return m_isUpdate; }

		void RegistTargetList(CActiveSceneCharacter* target_list);	//登録
		void RemoveTargetList(CActiveSceneCharacter* target_list);	//削除

		//リストの取得
		inline std::list<CActiveSceneCharacter*> GetTargetPlayerList() { return m_pTargetPlayerList; }	

		//カード情報の読み込み
		virtual void LoadInfo(int id);

		//固有情報の読み込み
		virtual void LoadUniqueInfo(CCard_Client::Param param) {}

		//inline CCard* GetTop() { return m_pTop; }
		//inline CCard* GetPrev() { return m_pPrev; }

		//列挙からゾーンのポインタを返す
		CZone* CastToEnumZone(ZONE zone, CDuelCharacter* duel);

		//ストラテジーの設定と取得
		void SetCardStrategy(CCardStrategy_Base* strategy);
		CCardStrategy_Base* GetCardStrategy() { return m_Strategy; }

		//使用者のエリアを返す
		inline void SetUserArea(CInputMouse::AREA area) { m_UserArea = area; }
		inline CInputMouse::AREA GetUserArea() { return m_UserArea; }

	private:

		//static CCard* m_pTop;	//先頭のオブジェクトポインタ
		//static CCard* m_pCur;	//現在（最後尾）のポインタ
		//CCard* m_pPrev;			//前のオブジェクトのポインタ
		//CCard* m_pNext;			//次のオブジェクトのポインタ

		BaseStatus m_BaseStatus;

		/**
		 * @brief ステートをチェンジできるかどうか
		 */
		bool m_IsChange;

		/**
		* どのプレイヤーに対して攻撃するのか
		* TODO : AttackCardのみに記載
		*/
		CInputMouse::AREA m_target;

		/**
		 * @brief カードを選択しているか否か(trueだったら選択している)
		 */
		bool m_IsChoice;

		/**
		 * @brief カードカードのタイプ
		 */
		CARDTYPE_ m_CardType;

		/**
		 * @brief カードの状態
		 */
		CCardState* m_pState;

		/**
		 * @brief カードの状態列挙
		 */
		My::CCardState::CARD_STATE m_StateNum;

		/**
		 * @brief ニュートラル状態の位置(戻すため)
		 */
		D3DXVECTOR3 m_NeutralPos;

		D3DXVECTOR3 m_outpos;

		CActiveScenePlayer* m_pCardHolder; //!<カードの所有者(一旦プレイヤーで固定)

		std::list<CActiveSceneCharacter*> m_pTargetPlayerList; //!<対象 NOTE:複数対象のカードが存在するためリストで管理

		/**
		 * @brief 更新するカードか
		 */
		bool m_isUpdate;

		/**
		 * @brief ゾーンの状態
		 */
		ZONE m_CurrentZone;	//現在のゾーン
		ZONE m_OldZone;		//昔のゾーン

		/**
		 * @brief カードのストラテジー
		 */
		CCardStrategy_Base* m_Strategy;

		/**
		* 使用者のエリア
		*/
		CInputMouse::AREA m_UserArea;
	};
};

#endif // !CARD_H_