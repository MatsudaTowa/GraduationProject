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
#include "BitStream.h"

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
			FLIP,
			CEMETERY
		};

		enum AssistType
		{// アシストの種類
			NONE_ASSIST,        // アシスト以外のときはこれ
			OBSTRUCT,            // 妨害
			BUFF,                // バフ
			DEBUFF                // デバフ
		};

		enum BuffType
		{// バフの種類 TODO:継承によって変わるかも
			NONE_BUFF,
			HEAL,                // HP回復
			NO_NAMEONE,            // 未定①
			NO_NAMETWO,            // 未定②
		};

		enum HealType
		{// 回復の種類 TODO:ヒール側のstrategyに持たせよう
			NONE_HEAL,                    // 回復しない
			ALL_HEAL,                    // 全体回復
			SPECIFIC_HEAL,                // 特定の相手を選んで回復
			RANDOM_HEAL,                // ランダム回復
			SELFINTARGET_HEAL,            // 自分を含めてランダム回復
			ONLY_ME,                    // 自分だけ
		};

		enum DebuffType
		{// デバフの種類 TODO:継承によって変わるかも
			NONE_DEBUFF,
			DEATH,                // 死亡
		};

		enum CastDestination
		{//キャスト先
			NONE = 0,	//無し
			AREA,		//エリア
			CARD,		//カード
			MAX
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
		virtual bool IsCast(CDuelCharacter* duel, CInputMouse::AREA area) = 0;

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
		//inline void SetTarget(CInputMouse::AREA area) { m_target = area; }
		//inline CInputMouse::AREA GetTarget() { return m_target; }

		/**
		 * @brief キャスト先の列挙取得
		 * @return キャスト先の列挙
		 */
		inline CastDestination GetCastDestination() { return m_CastDestination; }

		/**
		 * @brief キャスト先の列挙設定
		 * @param キャスト先の列挙値
		 */
		inline void SetCastDestination(CastDestination castdestination) { m_CastDestination = castdestination; }

		inline void SetCardHolder(CActiveScenePlayer* player) { m_pCardHolder = player; }
		inline CActiveScenePlayer* GetCardHolder() { return m_pCardHolder; }

		inline void SetUpdate(bool update) { m_isUpdate = update; }
		inline bool GetUpdate() { return m_isUpdate; }

		void RegistTargetList(CActiveSceneCharacter* target_list);	//登録
		void RemoveTargetList(CActiveSceneCharacter* target_list);	//削除
		void RemoveAllTargetList() { m_pTargetPlayerList.clear(); }	//全て削除

		//ターゲットリストの追加
		virtual void AddTargetPlayerList(CActiveSceneCharacter* character) { m_pTargetPlayerList.push_back(character); }

		//リストの取得
		inline std::list<CActiveSceneCharacter*> GetTargetPlayerList() { return m_pTargetPlayerList; }

		//カード情報の読み込み
		virtual void LoadInfo(int id);

		//固有情報の読み込み
		virtual void LoadUniqueInfo(CCard_Client::Param param) {}

		//inline CCard* GetTop() { return m_pTop; }
		//inline CCard* GetPrev() { return m_pPrev; }

		//引数のゾーンの列挙に対応したポインタを返す
		CZone* CastToZone(ZONE zone, CDuelCharacter* duel);

		//ストラテジーの設定と取得

		/**
		 * @brief 計算前の効果処理設定
		 * @param [in]strategy_vector
		 */
		void SetPreCalculateStrategyVector(std::vector<CCardStrategy_Base*> strategy_vector) { m_PreCalculateStrategy = strategy_vector; }
		
		/**
		 * @brief 計算後の効果処理設定
		 * @param [in]strategy_vector
		 */
		void SetpostCalculateStrategyVector(std::vector<CCardStrategy_Base*> strategy_vector) { m_PostCalculateStrategy = strategy_vector; }

		/**
		 * @brief 効果前の効果取得
		 * @return [in]m_PreCalculateStrategy
		 */
		std::vector<CCardStrategy_Base*> GetPreCalculateStrategyVector() { return m_PreCalculateStrategy; }

		/**
		 * @brief 効果後の効果取得
		 * @return [in]m_PreCalculateStrategy
		 */
		std::vector<CCardStrategy_Base*> GetPostCalculateVector() { return m_PostCalculateStrategy; }

		//使用者のエリアを返す
		inline void SetUserArea(CInputMouse::AREA area) { m_UserArea = area; }
		inline CInputMouse::AREA GetUserArea() { return m_UserArea; }

		//カードの使用者を記録
		void SetUserId(int id) { m_nUserId = id; }
		int GetUserId() { return m_nUserId; }

		//同種類の中のカード番号
		void SetSameTypeId(int id) { m_nSameTypeId = id; }
		int GetSameTypeId() { return m_nSameTypeId; }

		//キャスト開始時間
		void SetStartCastTime(uint64_t time) { m_fCastStartTime = time * 0.001f; }
		float GetStartCastTime() { return m_fCastStartTime; }

		//トリガーを受信した際の処理
		virtual void ReceiveTrigger();

		//カード情報の送信
		virtual void SendCardInfo(RakNet::BitStream* bsout) = 0;

		//カード情報の読み込み
		virtual void LoadCardInfo(RakNet::BitStream* bsin) = 0;

		//ステイ中に並べるカードか
		virtual bool IsSetUpStay() = 0;

		//ステイ時間を表示するか
		virtual bool IsCreatStayCount() = 0;

	private:

		//次の列挙から過去のゾーンのポインタを返す
		CZone* CastToEnumZone(ZONE zone, CDuelCharacter* duel);

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
		//CInputMouse::AREA m_target;

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

		My::CCardState::CARD_STATE m_OldStateNum;

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
		std::vector<CCardStrategy_Base*> m_PreCalculateStrategy;	//!<効果前の効果
		std::vector<CCardStrategy_Base*> m_PostCalculateStrategy;	//!<効果後の効果

		/**
		* 使用者のエリア
		*/
		CInputMouse::AREA m_UserArea;

		/**
		* 使用者の番号
		*/
		int m_nUserId;

		/**
		* 同じ種類の何番目のカードか
		*/
		int m_nSameTypeId;

		/**
		* キャスト時間
		*/
		float m_fCastStartTime;

		/**
		* キャスト先
		*/
		CastDestination m_CastDestination;
	};
};

#endif // !CARD_H_