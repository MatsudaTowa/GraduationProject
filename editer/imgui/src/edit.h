//***************************************************************************************************
//
// imgui(edit.h)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef _EDIT_H_
#define _EDIT_H_

// include
#include "main.h"
#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>
#include <type_traits>
#include "imgui/imgui.h"

/** @brief My 名前空間 */
namespace My
{
	enum RARITY
	{// レアリティの種類
		NONE_RARITY,
		COMMON,			// コモン
		UNCOMMON,		// アンコモン
		RARE,			// レア
		SUPERRARE,		// スーパーレア
		HYPERRARE,		// ハイパーレア
		ROYALRARE,		// ロイヤルレア
		EMPERORRARE,	// エンペラーレア
		GODRARE,		// ゴッドレア
		XRARE,			// エックスレア
	};

	enum CardType
	{// カードの種類
		NONE,
		ATTACK,
		DEFENSE,
		ASSIST,
	};

	enum AttackType
	{// 攻撃の種類
		NONE_ATTACK,			// 攻撃以外のときはこれ
		ALL_ATTACK,				// 全体攻撃
		SPECIFIC_ATTACK,		// 特定の相手を選んで攻撃
		RANDOM_ATTACK,			// ランダム攻撃
		SELFINTARGET_ATTACK,	// 自分を含めた攻撃
	};

	enum DefenseType
	{// 守備の種類
		NONE_DEFENSE,		// 守備以外のときはこれ
		COUNTER,			// 反撃できる
		NOT_COUNTER,		// 反撃できない
	};

	enum AssistType
	{// アシストの種類
		NONE_ASSIST,		// アシスト以外のときはこれ
		OBSTRUCT,			// 妨害
		BUFF,				// バフ
		DEBUFF				// デバフ
	};

	enum BuffType
	{// バフの種類
		NONE_BUFF,
		HEAL,				// HP回復
		NO_NAMEONE,			// 未定①
		NO_NAMETWO,			// 未定②
	};

	enum DeBuffType
	{// デバフの種類
		NONE_DEBUFF,
		DEATH,				// 死亡
	};

	enum HealType
	{// 回復の種類
		NONE_HEAL,					// 回復しない
		ALL_HEAL,					// 全体回復
		SPECIFIC_HEAL,				// 特定の相手を選んで回復
		RANDOM_HEAL,				// ランダム回復
		SELFINTARGET_HEAL,			// 自分を含めてランダム回復
		ONLY_ME,					// 自分だけ
	};

	struct Card
	{// カードの構造体
		std::string name;							// カード名
		std::string ruby;							// フリガナ
		int cost;									// カードコスト
		int damage;									// ダメージ数
		int guard;									// ガード値
		int counter;								// 反撃値
		int heal;									// 回復値
		bool isOneTime;								// 効果の発動時間(true = 単発,false = 単発じゃない)
		int time;									// 発動時間

		CardType maintype;							// カードの種類
		RARITY raritytype;							// レアリティの種類
		AssistType assisttype;						// アシストの種類
		AttackType attacktype;						// 攻撃の種類
		DefenseType defensetype;					// 守備の種類
		BuffType bufftype;							// バフの種類
		HealType healtype;							// 回復の種類
	
		std::string imagePath;						// 画像ファイルパス
		IDirect3DTexture9* ImageTexture = nullptr;	// カードイラストのテクスチャポインタ
		IDirect3DTexture9* TypeTexture = nullptr;	// カードタイプのテクスチャポインタ
		LPDIRECT3DTEXTURE9 typeIcon = nullptr;
	};

	struct Pack
	{// カードプールの構造体
		std::string Packname;						// パック名
		std::string Ruby;							// フリガナ
		std::vector<Card>cards;						// カードの情報
	};


	/** @brief My 編集クラス */
	class CEdit
	{
	public:
		static const int MAX_VERTEX = 4;	// ポリゴンの頂点数
		static const int MAX_EXTENSION = 2;	// 拡張子の数

		template<typename T>
		static inline T clamp(T v, T lo, T hi) 
		{
			return v < lo ? lo : (hi < v ? hi : v);
		}

		template<typename T>
		inline T ImClamp(T v, T mn, T mx)
		{
			return (v < mn) ? mn : (v > mx) ? mx : v;
		}

		/**
		* @brief コンストラクタ
		*/
		CEdit();
		
		/**
		* @brief デストラクタ
		*/
		~CEdit();

		/**
		* @brief 初期化処理
		* @return 成功したかどうか
		*/
		HRESULT Init(HWND hWnd,LPDIRECT3DDEVICE9 device);

		/**
		* @brief 終了処理
		*/
		void Uninit();

		/**
		* @brief 更新処理
		*/
		void Update();

		/**
		* @brief 編集処理
		*/
		void Edit();

		/**
		* @brief セーブ処理
		*/
		void Save();

		/**
		* @brief ロード処理
		*/
		void Load();

		/**
		* @brief テクスチャ設定
		* @param [in]プール番号
		* @param [in]カード番号
		*/
		void SetImage(int packID,int ID);

		/**
		* @brief コスト編集
		* @param [in]プール番号
		* @param [in]カード番号
		*/
		void EditCost(int packID,int ID);

		/**
		* @brief レアリティ編集
		* @param [in]プール番号
		* @param [in]カード番号
		*/
		void EditRarity(int packID,int ID);

		/**
		* @brief カードタイプ選択
		* @param [in]プール番号
		* @param [in]カード番号
		*/
		void SelectType(int packID,int ID);

		/**
		* @brief アシスト設定処理
		* @param [in]カード番号
		*/
		void SetAssist(int packID,int ID);

		/**
		* @brief 効果の発動時間
		* @param [in]パック番号
		* @param [in]カード番号
		*/
		void SetActiveTime(int PackID, int ID);

		/**
		* @brief 攻撃設定処理
		* @param [in]カード番号
		*/
		void SetAttack(int packID,int ID);

		/**
		* @brief 守備設定処理
		* @param [in]カード番号
		*/
		void SetDefense(int packID,int ID);

		/**
		* @brief プレビュー画面編集
		*/
		void EditPreview();

		/**
		* @brief デッキ画面選択
		*/
		void SelectDeckPreview();

		/**
		* @brief 相対パス抽出処理
		* @param [in]画像の絶対パス
		* @return 画像の相対パス
		*/
		std::string TrimTexturePath(const std::string& fullPath);

		/**
		* @brief 並べ替え処理
		*/
		void Soat();

		std::vector<Pack>& Packs() { return m_Pack; }
		const std::vector<Pack>& Packs() const { return m_Pack; }

		bool IsValidPackIndex(size_t p)const { return p < m_Pack.size(); }
		bool IsValidCardIndex(size_t p, size_t c) const
		{
			return IsValidPackIndex(p) && c < m_Pack[p].cards.size();
		}

		// パック／カード管理
		size_t CreatePack(const std::string& PackName,const std::string&PackNameRuby) 
		{
			Pack p; p.Packname = PackName;
			p.Ruby = PackNameRuby;
			m_Pack.emplace_back(std::move(p));
			return m_Pack.size() - 1;
		}

		void AddCardToPack(size_t packIndex, const Card& card) 
		{
			if (!IsValidPackIndex(packIndex)) return;
			m_Pack[packIndex].cards.push_back(card);
		}
		bool RemoveCardFromPack(size_t packIndex, size_t cardIndex)
		{
			if (!IsValidCardIndex(packIndex, cardIndex)) return false;
			auto& v = m_Pack[packIndex].cards;
			v.erase(v.begin() + cardIndex);
			return true;
		}

		bool RemovePack(size_t PackIndex);

		/**
		* @brief 読み込んだ画像設定処理
		* @param [in]カード情報
		*/
		void SetLoadTexture(Card& card);

		/**
		* @brief パック名保存処理
		*/
		void SavePackName();

		/**
		* @brief パック名ロード処理
		*/
		void LoadPackName();


		// 前後空白を除去したコピーを返す
		inline std::string trim_copy(const std::string& s)
		{
			if (s.empty()) return s;

			auto l = s.begin();
			while (l != s.end() && std::isspace(static_cast<unsigned char>(*l))) ++l;

			auto r = s.end();
			do { --r; } while (r != l && std::isspace(static_cast<unsigned char>(*r)));
			++r; // r は末尾の次を指す

			return std::string(l, r);
		}

	private:
		/**
		* @brief バックバッファスケール設定
		*/
		void UpdateScale();

		/**
		* @brief ファイル設定
		* @param [in]ウィンドウハンドル
		*/
		std::string OPenImageDialog(HWND hWnd);

		/**
		* @brief カード背景設定 
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetBG(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief カードフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetCardFrame(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief カードイラスト設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetCardIrust(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief カードイラストフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetCardIrustFrame(int PackID, int CardID, ImVec2 pos);

		/**
		* @brief カードネームフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetNameFrame(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief テキストフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetTxtFrame(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief テキストアイコン設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/


		/**
		* @brief タイプフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetTypeFrame(int PackID,int CardID,ImVec2 pos);

		/**
		* @brief タイプアイコン設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param [in]位置
		*/
		void SetTypeIcon(int PackID, int CardID, ImVec2 pos);

		/**
		* @brief コストフレーム設定
		* @param [in]パック番号
		* @param [in]カード番号
		* @param[in]位置
		*/
		void SetCostFrame(int PackID,int CardID,ImVec2 pos);

		HWND m_hWnd;

		int m_ID;
		int m_Step;
		int m_Select;	

		IDirect3DTexture9* m_pImageTexture;
		LPDIRECT3DDEVICE9 m_pDevice;

		std::vector<Card> m_CardList;
		std::string m_SelectedImagePath;	// 画像のファイルパス
	
		int prevType = -1;					// 前回のカードタイプ

		IDirect3DTexture9* m_PlaceholderTex;				// 画像設定していないとき専用のテクスチャポインタ
		IDirect3DTexture9* m_IrustBG = nullptr;				// カード背景(色変える部分)
		IDirect3DTexture9* m_CardFrameMask = nullptr;		// カード背景(色変えない部分)
		IDirect3DTexture9* m_IllustrationFrame = nullptr;	// カードイラストフレーム
		IDirect3DTexture9* m_CardNameFrame = nullptr;		// カードネームフレーム
		IDirect3DTexture9* m_CardTxtFrame = nullptr;		// カードテキストフレーム
		IDirect3DTexture9* m_CardTypeFrame = nullptr;		// カードタイプフレーム
		IDirect3DTexture9* m_CardCostFrame = nullptr;		// カードコストフレーム

		ImFont* m_Font = nullptr;			// 通常サイズ
		ImFont* m_LargeFont = nullptr;		// 大サイズ
		ImFont* m_NumberFont = nullptr;		// 数字を表示する専用のフォント

		//**********************
		// パック用メンバ変数
		//**********************
		std::vector<Pack>m_Pack;
		int m_SelectPack;

		std::string m_NewPackName;			// モーダル用に編集中の名前
		bool m_ShowNewPackModal = false;	// モーダル開閉フラグ
	
		int m_EditPackIndex;
		char m_PackNameBuf[256] = {};
		char m_PackRubyBuf[256] = {};

		int m_PendingReorderSrc = -1;
		int m_PendingReorderDst = -1;
	
		bool m_bWindowSizeDeck;	// ウィンドウサイズを変えたかどうか(デッキ編集画面)
		bool m_bWindowSizeCard;	// ウィンドウサイズを変えたかどうか(カード編集画面)
};
}

static const float CARDFRAME_WIDTH = 300.0f;	// フレームイラストの幅
static const float CARDFRAME_HEIGHT = 450.0f;	// フレームイラストの高さ
static const float FRAME_PADDING = 15.0f;		// フレームイラストとイラストの隙間
static const float ADJUST_APPEAR = 30.0f;		// 表示位置の調整

static const float IRUST_SIZE = 290.0f;			// イラストの大きさ
static const float IRUST_PADDING = 25.0f;		// カード端とイラストの隙間

static const float NAMEFRAME_WIDTH = 350.0f;	// ネームフレームの幅
static const float NAMEFRAME_HEIGHT = 60.0f;	// ネームフレームの高さ

static const float TXTFRAME_WIDTH = 300.0f;		// テキストフレームの幅
static const float TXTFRAME_HEIGHT = 200.0f;	// テキストフレームの高さ

static const float TYPEFRAME_WIDTH = 120.0f;	// タイプフレームの幅
static const float TYPEFRAME_HEIGHT = 120.0f;	// タイプフレームの高さ

static const float TYPEICON_WIDTH = 45.0f;		// タイプアイコンの幅(直径)
static const float TYPEICON_HEIGHT = 45.0f;		// タイプアイコンの高さ(直径)

static const float COSTFRAME_WIDTH = 100.0f;	// コストフレームの幅
static const float COSTFRAME_HEIGHT = 100.0f;	// コストフレームの高さ

static const float TURN_QUATER = -0.25f;		// 45度回転
#endif