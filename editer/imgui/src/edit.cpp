//***************************************************************************************************
//
// imgui(edit.cpp)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef NOMINMAX
#define NOMINMAX    // windows.h の min/max マクロ定義を抑制
#endif

#define IMGUI_DEFINE_MATH_OPERATORS

// include 
#include "edit.h"
#include "manager.h"
#include "imgui/image/tinyfiledialogs.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/misc/imgui_stdlib.h"
#include <imm.h>
#include <algorithm>  
#include <iostream>
#include <map>
#include <sstream>
#include "nlohmann/json.hpp"
#include "imgui/imgui_internal.h"
#include <ShObjIdl.h>
#pragma comment(lib,"imm32")

// for convenience
using ordered_json = nlohmann::ordered_json;

// std::string → std::wstring に変換する関数
std::wstring ConvertToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], len);
	return wstr;
}

/**
 * @brief コンストラクタ
 */
My::CEdit::CEdit() :
	m_ID(0),
	m_Step(0),
	m_Select(0),
	m_pImageTexture(nullptr),
	m_CardFrameMask(nullptr),
	m_IllustrationFrame(nullptr),
	m_CardNameFrame(nullptr),
	m_IrustBG(nullptr),
	m_CardTxtFrame(nullptr),
	m_CardTypeFrame(nullptr),
	m_CardCostFrame(nullptr),
	m_Font(nullptr),
	m_LargeFont(nullptr),
	m_NumberFont(nullptr),
	m_SelectPack(0),
	m_PlaceholderTex(),
	m_bWindowSizeDeck(false),
	m_bWindowSizeCard(false)
{
	
}

/**
 * @brief デストラクタ
 */
My::CEdit::~CEdit()
{
	m_ID = 0;
	m_Step = 0;
	m_Select = 0;
	m_pImageTexture = nullptr;
	m_PlaceholderTex = nullptr;
	m_CardTxtFrame = nullptr;
	m_bWindowSizeDeck = false;
	m_bWindowSizeCard = false;
	m_CardTypeFrame = nullptr;
	m_CardCostFrame = nullptr;
	m_Font = nullptr;
	m_LargeFont = nullptr;
	m_NumberFont = nullptr;
}

/**
 * @brief 初期化処理
 */
HRESULT My::CEdit::Init(HWND hWnd, LPDIRECT3DDEVICE9 device)
{
	// 変数の初期化
	m_ID = 1;				// カードID
	m_Step = 1;				// 数字の増減幅
	m_Select = -1;	// 消すカードの選択肢

	m_pDevice = device;
	m_hWnd = hWnd;

	// IMGUI初期化
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;	// ファイルを無効化
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(device);

	// 日本語フォントを追加(サイズ小)
	m_Font = io.Fonts->AddFontFromFileTTF(
		"data\\fonts\\meiryo-bold.ttf",
		24.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese()
	);

	// 日本語フォントを追加(サイズ大)
	m_LargeFont = io.Fonts->AddFontFromFileTTF(
		"data\\fonts\\H7GBKHeavy.TTF",
		28.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese()
	);

	// 日本語フォントを追加(数字専用)
	m_NumberFont = io.Fonts->AddFontFromFileTTF(
		"data\\fonts\\H7GBKHeavy.TTF",
		48.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese()
	);

	io.FontDefault = m_Font;

	// ImGuiのボタンの色を変える
	ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	// IMEを関連付け
	HIMC hIMC = ImmGetContext(hWnd);
	ImmAssociateContext(hWnd, hIMC);

	if (m_PlaceholderTex == nullptr)
	{// 画像が読み込まれていないとき(画像選択されていないときに使う画像)
		std::wstring phPath = ConvertToWString("data\\TEXTURE\\image.png"); // 用意するプレースホルダ画像
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, phPath.c_str(), &m_PlaceholderTex)))
		{
			m_PlaceholderTex = nullptr;
		}
	}

	// カード情報ロード処理
	Load();

	// パック情報ロード処理
	LoadPackName();

	return S_OK;
}

/**
 * @brief 終了処理
 */
void My::CEdit::Uninit()
{
	// --- ImGui のシャットダウン（元の処理） ---
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// --- テクスチャ等の Direct3D リソースを確実に解放 ---
	// 各パック内のカードが保持する ImageTexture を解放する
	for (auto& pack : m_Pack)
	{
		for (auto& c : pack.cards)
		{
			if (c.ImageTexture && c.ImageTexture != m_PlaceholderTex)
			{
				c.ImageTexture->Release();
				c.ImageTexture = nullptr;
			}
		}
	}

	// 共有プレースホルダテクスチャは最後に一度だけ解放する
	if (m_PlaceholderTex)
	{
		m_PlaceholderTex->Release();
		m_PlaceholderTex = nullptr;
	}
}

/**
 * @brief 更新処理
 */
void My::CEdit::Update()
{

}

/**
* @brief 編集処理
*/
void My::CEdit::Edit()
{
	// IMGUIフレーム開始
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	UpdateScale();  // スケール調整

	ImGui::PushFont(ImGui::GetIO().FontDefault);

	// 左側 : デッキ選択 (SelectDeckPreview が m_SelectedPack を設定する想定)
	SelectDeckPreview();

	// ウィンドウサイズの初期化
	if (!m_bWindowSizeCard)
	{
		ImGui::SetNextWindowSize(ImVec2(1100.0f, 700.0f), ImGuiCond_Always);
		m_bWindowSizeCard = true;
	}

	ImGui::SetNextWindowPos(ImVec2(600, 50), ImGuiCond_FirstUseEver);

	// メインウィンドウ
	ImGui::Begin("Card data");

	ImGui::SameLine();

	// 中央 : カードデータ編集ビュー
	ImGui::BeginChild("Editor", ImVec2(600, 0), true);

	// 選択パックが有効か確認
	if (!IsValidPackIndex(m_SelectPack))
	{
		ImGui::Text(u8"パックが一つもありません。");
		ImGui::Text(u8"パックを設定してください。");
	}
	else
	{
		auto& cards = m_Pack[m_SelectPack].cards;

		if (ImGui::Button(u8"追加"))
		{
			// 新規カードをパックに追加
			cards.push_back(Card{ u8"名前なし",u8"名前なし", 0, 0, 0, 0, 0, CardType::ATTACK, RARITY::NONE_RARITY,AssistType::NONE_ASSIST,
								  AttackType::NONE_ATTACK, DefenseType::NONE_DEFENSE, BuffType::NONE_BUFF,HealType::NONE_HEAL, "", nullptr});
			m_Select = (int)cards.size() - 1;
		}

		ImGui::SameLine();

		if (ImGui::Button(u8"削除") &&
			m_Select >= 0 &&
			m_Select < (int)cards.size())
		{
			cards.erase(cards.begin() + m_Select);
			m_Select = -1;
		}

		ImGui::SameLine();

		if (ImGui::Button(u8"セーブ"))
		{
			if (!cards.empty())
			{
				Save(); // カード情報保存

				SavePackName();	// パック名保存
			}
			else
			{
				assert("カードが1枚もありません");
			}
		}

		// カード一覧と編集
		ImGui::BeginChild("CardList", { 0, 0 }, true);

		for (int i = 0; i < (int)cards.size(); ++i)
		{
			ImGui::PushID(i);


			if (ImGui::Selectable(cards[i].name.c_str(), m_Select == i))
			{
				m_Select = i;
			}

			// drag source（既存コード）
			int dragIndex = i;
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("CARD_REORDER_SIMPLE", &dragIndex, sizeof(int));
				ImGui::Text("%s", cards[i].name.c_str());
				ImGui::EndDragDropSource();
			}

			// drag target: payload を受け取ったら pending にセットして Soat() を呼ぶ
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CARD_REORDER_SIMPLE"))
				{
					if (payload->DataSize == sizeof(int))
					{
						int src = *(const int*)payload->Data;
						int dst = i;
						m_PendingReorderSrc = src;
						m_PendingReorderDst = dst;
						Soat(); // 引数無しで並べ替えを実行
					}
				}
				ImGui::EndDragDropTarget();
			}


			if (ImGui::TreeNode(u8"編集"))
			{
				// カード名の編集
				ImGui::InputText(u8"カード名", &cards[i].name, ImGuiInputTextFlags_EnterReturnsTrue);

				// フリガナの編集
				ImGui::InputText(u8"カードの読み方", &cards[i].ruby, ImGuiInputTextFlags_EnterReturnsTrue);

				SetImage(m_SelectPack, i);		// 使用画像編集
				EditCost(m_SelectPack, i);		// コスト編集
				EditRarity(m_SelectPack, i);	// レアリティ編集
				SelectType(m_SelectPack, i);	// タイプ選択

				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		ImGui::EndChild();
	}

	ImGui::EndChild(); // Editor 終了

	ImGui::SameLine();

	// プレビュー画面編集（パックとカードの組み合わせで描画する想定）
	EditPreview(); 

	ImGui::End(); // メインウィンドウ

	ImGui::PopFont();

	// 描画
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

/**
* @brief セーブ処理
*/
void My::CEdit::Save()
{
	// ルート JSON を作る
	ordered_json root;
	root[u8"カード"] = ordered_json::array();

	// 列挙型を文字列に変換する配列（必要に応じて拡張してください）
	static const char* CardTypeNames[] =
	{// カードタイプ
		"NONE",
		"ATTACK",
		"DEFENSE",
		"ASSIST"
	};

	static const char* CardRarity[] =
	{// レアリティ
		"NONE_RARITY",
		"COMMON",
		"UNCOMMON",
		"RARE",
		"SUPERRARE",
		"HYPERRARE",
		"ROYALRARE",
		"EMPERORRARE",
		"GODRARE",
		"XRARE"
	};

	static const char* CardAttackTypes[] =
	{// 攻撃対象用タグ
		"NULL",
		"ALL_ATTACK",
		"SPECIFIC_ATTACK",
		"RANDOM_ATTACK",
		"SELFINTARGET_ATTACK",
	};

	static const char* CardDefenseTypes[] =
	{// 守備用タグ
		"NULL",
		"COUNTER",
		"NOT_COUNTER"
	};

	static const char* CardAssistType[] =
	{// アシスト用タグ
		"NONE_ASSIST",
		"OBSTRUCT",
		"BUFF",
		"DEBUFF"
	};

	static const char* CardBuffType[] =
	{// バフ用タグ
		"NONE_BUFF",
		"HEAL",
		"NONAME_ONE",
		"NONAME_TWO"
	};

	static const char* CardHealTypes[] =
	{// 回復対象用タグ
		"NULL",
		"ALL_HEAL",
		"SPECIFIC_HEAL",
		"RANDOM_HEAL",
		"SELFINTARGET_HEAL",
		"ONLY_ME"
	};


	// ヘルパ：前後空白を除去
	auto trim_copy = [](const std::string& s) -> std::string {
		size_t l = s.find_first_not_of(" \t\n\r");
		if (l == std::string::npos) return std::string();
		size_t r = s.find_last_not_of(" \t\n\r");
		return s.substr(l, r - l + 1);
	};

	// すべてのパックとカードを巡回して JSON 配列に詰める
	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{
		const auto& pack = m_Pack[packIdx];
		for (size_t cardIdx = 0; cardIdx < pack.cards.size(); ++cardIdx)
		{
			const auto& card = pack.cards[cardIdx];

			// カード名保存
			std::string nameToSave = card.name;
			if (!pack.Packname.empty())
			{
				std::string prefix = pack.Packname + "_";
				if (nameToSave.size() >= prefix.size() && nameToSave.compare(0, prefix.size(), prefix) == 0)
				{
					nameToSave = nameToSave.substr(prefix.size());
				}
			}
			// 前後空白トリム
			nameToSave = trim_copy(nameToSave);

			// ルビの保存
			std::string nameRubyToSave = card.ruby;
			if (!pack.Packname.empty())
			{
				std::string prefix = pack.Packname + "_";
				if (nameRubyToSave.size() >= prefix.size() && nameRubyToSave.compare(0, prefix.size(), prefix) == 0)
				{
					nameRubyToSave = nameRubyToSave.substr(prefix.size());
				}
			}
			// 前後空白トリム
			nameRubyToSave = trim_copy(nameRubyToSave);

			// 空ならデフォルト名（安全策）
			if (nameRubyToSave.empty())
			{
				nameRubyToSave = std::string("");
			}

			ordered_json j;
			j["Pack ID"] = (int)packIdx + 1;
			j["Card ID"] = (int)cardIdx + 1;
			j["Card Name"] = nameToSave;
			j["Card Name Ruby"] = nameRubyToSave;

			// 画像パス（相対パスを期待するなら card.imagePath を直接保存）
			j["image"] = card.imagePath;

			// 基本数値フィールド
			j["cost"] = card.cost;
			//j["rarity"] = card.rarity;

			// カードレアリティ
			// rarity
			{
				int rarity = static_cast<int>(card.raritytype);
				if(rarity >= 0 && rarity < (int)(sizeof(CardRarity) / sizeof(CardRarity[0])))
				{
					j["rarity"] = rarity;
				}
				else
				{
					j["rarity"] = "";
				}
			}

			// カードタイプ
			// maintype
			{
				int mt = static_cast<int>(card.maintype);
				if (mt >= 0 && mt < (int)(sizeof(CardTypeNames) / sizeof(CardTypeNames[0])))
				{
					j["type"] = mt;
				}
				else
				{
					j["type"] = "";
				}
			}

			switch (card.maintype)
			{
			case ATTACK:
				// 攻撃タイプ（必要なら追加）
			{
				int at = static_cast<int>(card.attacktype);	// 攻撃対象を設定
				if (at >= 0 && at < (int)(sizeof(CardAttackTypes) / sizeof(CardAttackTypes[0])))
				{// 選択肢の中から選んだ場合
					j["attacktype"] = at;
				}
				else
				{// 選択肢の中から選んでない場合
					j["attacktype"] = "NULL";
				}

				j["power"] = card.damage;	// 攻撃力の設定

			}

				break;
			case DEFENSE:
				// 守備タイプ
			{
				int dt = static_cast<int>(card.defensetype);	// 守備のタイプを設定
				if (dt >= 0 && dt < (int)(sizeof(CardDefenseTypes) / sizeof(CardDefenseTypes[0])))
				{
					j["defensetype"] = dt;
				}
				else
				{
					j["defensetype"] = "NULL";
				}

				j["guard"] = card.guard;
				if (card.defensetype == COUNTER)
				{
					j["counter"] = card.counter;
				}


			}

				break;
			case ASSIST:
				// アシストタイプ / バフタイプ
			{
				int at = static_cast<int>(card.assisttype);
				if (at >= 0 && at < (int)(sizeof(CardAssistType) / sizeof(CardAssistType[0])))
				{
					j["assisttype"] = at;
				}
				else
				{
					j["assisttype"] = "NONE_ASSIST";
				}

				int bt = static_cast<int>(card.bufftype);
				if (bt >= 0 && bt < (int)(sizeof(CardBuffType) / sizeof(CardBuffType[0])))
				{
					j["bufftype"] = bt;
				}
				else
				{
					j["bufftype"] = "NONE_BUFF";
				}

				if (card.assisttype == BUFF)
				{
					switch (card.bufftype)
					{
					case HEAL:
						// 回復タイプのカードの場合
						int at = static_cast<int>(card.healtype);	// 回復対象を設定
						if (at >= 0 && at < (int)(sizeof(CardHealTypes) / sizeof(CardHealTypes[0])))
						{// 選択肢の中から選んだ場合
							j["healtype"] = at;
						}
						else
						{// 選択肢の中から選んでない場合
							j["healtype"] = "NULL";
						}

						j["heal"] = card.heal;
						break;
					}
				}

			}

				break;
			}

			// 最終的に配列へ追加
			root[u8"カード"].push_back(j);
		}
	}

	// ファイル書き込み
	std::ofstream ofs("data\\json\\cards.json");

	if (!ofs.is_open())
	{// 書き出しに失敗した場合
		MessageBox(m_hWnd, L"セーブに失敗しました", L"失敗ログ", MB_OK);
		return;
	}
	else
	{// 書き出しに成功した場合
		MessageBox(m_hWnd, L"セーブに成功しました!!", L"成功ログ", MB_OK);
	}
	ofs << root.dump(4) << std::endl;
}

/**
* @brief パック名保存処理
*/
void My::CEdit::SavePackName()
{
	// ルート JSON を作る
	ordered_json root;
	root[u8"パック"] = ordered_json::array();

	// 各パックの情報を詰める
	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{
		const auto& pack = m_Pack[packIdx];

		// パック名を前後トリムし、空ならデフォルト名を設定
		std::string packName = trim_copy(pack.Packname);
		if (packName.empty())
		{
			packName = std::string(u8"");
		}

		// パック名ルビを前後トリムし、空ならデフォルト名を設定
		std::string packNameRuby = trim_copy(pack.Ruby);
		if (packNameRuby.empty())
		{
			packNameRuby = std::string(u8"");
		}

		ordered_json pj;
		pj["Pack Name"] = packName;
		pj["Pack Name Ruby"] = packNameRuby;

		root[u8"パック"].push_back(pj);
	}

	// 書き出し先ファイル（必要ならパスを変更）
	const std::string path = "data\\json\\packs.json";

	// ファイル出力
	std::ofstream ofs(path, std::ios::trunc);
	if (!ofs.is_open())
	{
		MessageBox(m_hWnd, L"パック名のセーブに失敗しました", L"失敗ログ", MB_OK);
		return;
	}

	ofs << root.dump(4) << std::endl;
	ofs.close();

	//MessageBox(m_hWnd, L"パック名をセーブしました", L"成功ログ", MB_OK);
}

/**
* @brief ロード処理
*/
void My::CEdit::Load()
{
	const std::string relPath = "data\\json\\cards.json";	// ファイルを設定

	// デバッグ: 実行時のカレントディレクトリと読み込むファイルの絶対パスを出力すると早く原因が分かります
	std::cout << "Attempting to load file: " << relPath << "\n";

	// テキストで全読み込み
	std::ifstream ifs(relPath.c_str());
	if (!ifs.is_open())
	{
		MessageBox(m_hWnd, L"ファイルが存在しません", L"エラー", MB_OK);
		std::cout << "Failed to open: " << relPath << "\n";
		return;
	}

	std::string fileData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	// 先頭3バイトをデバッグ表示（BOM検出）
	if (fileData.size() >= 3)
	{
		unsigned char b0 = static_cast<unsigned char>(fileData[0]);
		unsigned char b1 = static_cast<unsigned char>(fileData[1]);
		unsigned char b2 = static_cast<unsigned char>(fileData[2]);
		std::cout << "首3バイト (hex): "
			<< std::hex << std::setw(2) << std::setfill('0') << (int)b0 << " "
			<< std::setw(2) << (int)b1 << " "
			<< std::setw(2) << (int)b2 << std::dec << "\n";
	}

	// BOM (EF BB BF) を取り除く（あるとキー比較で失敗することがある）
	if (fileData.size() >= 3 &&
		static_cast<unsigned char>(fileData[0]) == 0xEF &&
		static_cast<unsigned char>(fileData[1]) == 0xBB &&
		static_cast<unsigned char>(fileData[2]) == 0xBF)
	{
		std::cout << "UTF-8 BOM detected; removing\n";
		fileData.erase(0, 3);
	}

	ordered_json Json;

	try
	{
		// 文字列から直接パース(日本語変換)
		Json = ordered_json::parse(fileData);
	}
	catch (const std::exception& e)
	{
		MessageBox(m_hWnd, L"JSON のパースに失敗しました", L"エラー", MB_OK);
		std::cout << "JSON parse error: " << e.what() << "\n";
		return;
	}

	// ルート情報とキー一覧を出力（デバッグ）
	std::cout << "Root type: " << Json.type_name() << "\n";
	std::cout << "Root dump:\n" << Json.dump(2) << "\n";

	if (Json.is_object())
	{
		std::cout << "Root keys and their byte sequences:\n";
		for (auto it = Json.begin(); it != Json.end(); ++it)
		{
			const std::string k = it.key();
			std::cout << " Key: [" << k << "] type: " << it.value().type_name() << " bytes:";
			for (unsigned char c : k) {
				std::cout << " " << std::hex << std::setw(2) << std::setfill('0') << (int)c;
			}
			std::cout << std::dec << "\n";
		}
	}

	// "カード" キーを厳密に探す（不可視文字や近似キーも拾う）
	std::string cardKey;
	if (Json.contains(u8"カード")) {
		cardKey = u8"カード";
		std::cout << "Exact key u8\"カード\" found via contains()\n";
	}
	else if (Json.is_object())
	{
		// 逐一比較して類似キーを探す（暫定）
		for (auto it = Json.begin(); it != Json.end(); ++it)
		{
			const std::string k = it.key();
			if (k == u8"カード") { cardKey = k; break; }
			// 部分一致も許容する（不要なら削除）
			if (k.find(u8"カード") != std::string::npos) { cardKey = k; break; }
		}
	}

	ordered_json cardNode;
	if (!cardKey.empty()) cardNode = Json[cardKey];
	else if (Json.is_array()) cardNode = Json; // ルートが配列ならそのまま使う

	if (cardNode.is_null())
	{
		MessageBox(m_hWnd, L"\"カード\"キーが存在しません", L"エラー", MB_OK);
		std::cout << "\"カード\" key not found after scanning\n";
		return;
	}

	// m_Pack 初期化
	m_Pack.clear();

	// カード処理ラムダ（既存ロジックと同様に保持）
	auto ProcessCardJson = [this](const ordered_json& j)
	{
		int packID = j.value("Pack ID", 1);
		size_t packIndex = (packID > 0) ? static_cast<size_t>(packID - 1) : 0;

		while (m_Pack.size() <= packIndex)
		{
			Pack pack;
			pack.Packname = "Pack" + std::to_string(m_Pack.size() + 1);
			m_Pack.push_back(pack);
		}

		// 読み込む情報の初期化
		Card card;
		card.name = j.value("Card Name", j.value("CardName", "名前なし"));	// カード名
		card.ruby = j.value("Card Name Ruby", j.value("Card Name Ruby", "名前なし"));	// カード名の読み方
		card.imagePath = trim_copy(j.value("image", j.value("Card Img", "")));
		card.cost = j.value("cost", 0);										// コスト
		card.raritytype = static_cast<My::RARITY>(j.value("rarity", 0));		// レアリティ
		card.maintype = static_cast<My::CardType>(j.value("type", 0));		// カードタイプ

		//********************************************
		// jsonファイルの情報からカードにタグ付け
		//********************************************

		switch (card.maintype)
		{
		case 1:	// 攻撃タイプ
			card.attacktype = static_cast<My::AttackType>(j.value("attacktype", 0));

			switch (card.attacktype)
			{
			case 1:	// 全体攻撃
				break;
			case 2:	// 特定の相手を選んで攻撃
				break;
			case 3:	// ランダム攻撃
				break;
			case 4:	// 自分を含めてランダム攻撃
				break;
			}

			card.damage = j.value("power", 0);									// 攻撃力

			break;
		case 2:	// 守備タイプ
			card.defensetype = static_cast<My::DefenseType>(j.value("defensetype", 0));
			switch (card.defensetype)
			{
			case 1:	// カウンターあり
				card.counter = j.value("counter", 0);								// カウンター値
				break;
			case 2:	// カウンター無し
				break;
			}

			card.guard = j.value("guard", 0);									// ガード値

			break;
		case 3:	// アシストタイプ
			card.assisttype = static_cast<My::AssistType>(j.value("assisttype", 0));
			switch (card.assisttype)
			{
			case 1:	// 妨害
				break;
			case 2:	// バフ

				card.bufftype = static_cast<My::BuffType>(j.value("bufftype", 0));	// バフタイプ

				switch (card.bufftype)
				{
				case 1:	// 回復タイプの時
					card.healtype = static_cast<My::HealType>(j.value("healtype", 0));

					card.heal = j.value("heal", 0);										// 回復量
					break;
				case 2:	// 未定①
					break;
				case 3:	// 未定②
					break;
				}
				break;
			case 3:	// デバフ
				break;
			}



			break;
		}

		m_Pack[packIndex].cards.push_back(card);
	};

	// cardNode を配列/オブジェクトどちらでも処理
	if (cardNode.is_array())
	{
		for (const auto& j : cardNode) { if (j.is_object()) ProcessCardJson(j); }
	}
	else if (cardNode.is_object())
	{
		for (auto it = cardNode.begin(); it != cardNode.end(); ++it)
		{
			const ordered_json& j = it.value();
			if (j.is_object()) ProcessCardJson(j);
		}
	}
	else
	{
		MessageBox(m_hWnd, L"\"カード\"の形式が不正です", L"エラー", MB_OK);
		return;
	}

	for (size_t p = 0; p < m_Pack.size(); ++p)
	{
		for (size_t c = 0; c < m_Pack[p].cards.size(); ++c)
		{
			SetLoadTexture(m_Pack[p].cards[c]); // ← ここで呼ぶ
		}
	}
	MessageBox(m_hWnd, L"カードデータの読み込みに成功しました！", L"読み込み完了", MB_OK);

}

/**
* @brief バック名ロード処理
*/
void My::CEdit::LoadPackName()
{
	const std::string relPath = "data\\json\\packs.json";

	std::cout << "Attempting to load pack names file: " << relPath << "\n";

	std::ifstream ifs(relPath.c_str(), std::ios::binary);
	if (!ifs.is_open())
	{
		MessageBox(m_hWnd, L"パック名ファイルが見つかりません", L"読み込み", MB_OK);
		std::cout << "Failed to open: " << relPath << "\n";
		return;
	}

	std::string fileData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	// BOM 検出と削除 (UTF-8 BOM: EF BB BF)
	if (fileData.size() >= 3 &&
		static_cast<unsigned char>(fileData[0]) == 0xEF &&
		static_cast<unsigned char>(fileData[1]) == 0xBB &&
		static_cast<unsigned char>(fileData[2]) == 0xBF)
	{
		std::cout << "UTF-8 BOM detected in pack names file; removing\n";
		fileData.erase(0, 3);
	}

	ordered_json root;
	try
	{
		root = ordered_json::parse(fileData);
	}
	catch (const std::exception& e)
	{
		MessageBox(m_hWnd, L"パック名ファイルのパースに失敗しました", L"読み込みエラー", MB_OK);
		std::cout << "JSON parse error (packs): " << e.what() << "\n";
		return;
	}

	// "パック" 配列取得（ルートが配列の場合も許容）
	ordered_json packArray;
	if (root.is_object() && root.contains(u8"パック") && root[u8"パック"].is_array())
	{
		packArray = root[u8"パック"];
	}
	else if (root.is_array())
	{
		packArray = root;
	}
	else
	{
		MessageBox(m_hWnd, L"パック情報が見つかりません", L"読み込みエラー", MB_OK);
		std::cout << "\"パック\" key not found or not an array\n";
		return;
	}

	// m_Pack を必要なら拡張（既存カードは保持）
	if (m_Pack.size() < packArray.size())
	{
		size_t old = m_Pack.size();
		m_Pack.resize(packArray.size());
		for (size_t i = old; i < m_Pack.size(); ++i)
		{
			m_Pack[i].Packname = std::string(u8"無名パック") + std::to_string(i + 1);
		}
	}

	// 各要素を読み込み Packname に設定
	for (size_t i = 0; i < packArray.size(); ++i)
	{
		const auto& pj = packArray[i];
		std::string name;
		std::string nameRuby;

		if (pj.is_object())
		{
			// パック名
			if (pj.contains("Pack Name") && pj["Pack Name"].is_string())
			{
				name = pj["Pack Name"].get<std::string>();
			}
			else if (pj.contains("PackName") && pj["PackName"].is_string())
			{
				name = pj["PackName"].get<std::string>();
			}
			else if (pj.contains("Pack") && pj["Pack"].is_string())
			{
				name = pj["Pack"].get<std::string>();
			}

			// パック名読み方
			if (pj.contains("Pack Name Ruby") && pj["Pack Name Ruby"].is_string())
			{
				nameRuby = pj["Pack Name"].get<std::string>();
			}
			else if (pj.contains("PackNameRuby") && pj["PackNameRuby"].is_string())
			{
				nameRuby = pj["PackNameRuby"].get<std::string>();
			}
			else if (pj.contains("Pack") && pj["Pack"].is_string())
			{
				nameRuby = pj["Pack"].get<std::string>();
			}
		}
		else if (pj.is_string())
		{
			name = pj.get<std::string>();
			nameRuby = pj.get<std::string>();
		}

		// trim_copy がプロジェクトにある前提
		name = trim_copy(name);
		nameRuby = trim_copy(nameRuby);

		if (name.empty())
		{
			name = std::string(u8"無名パック");
		}

		// m_Pack の該当エントリに設定（m_Pack は既に resize 済み）
		m_Pack[i].Packname = name;
		m_Pack[i].Ruby = nameRuby;

		std::cout << "Loaded Pack[" << i << "] = " << name << "\n";
	}

	// 少なくとも 1 パックは存在させ、先頭が空ならデフォルト名を入れる
	if (m_Pack.empty())
	{
		Pack p;
		p.Packname = std::string(u8"無名パック1");
		m_Pack.push_back(std::move(p));
	}
	else
	{
		if (trim_copy(m_Pack[0].Packname).empty())
			m_Pack[0].Packname = std::string(u8"無名パック1");
	}

	// UI 側の表示用インデックスを先頭にリセットして確実に表示されるようにする
	m_SelectPack = 0;
	m_Select = -1;
	// 編集モードは解除しておく（起動時に自動で編集に入らない）
	m_EditPackIndex = -1;

	std::cout << "Finished loading pack names. m_Pack.size()=" << m_Pack.size() << "\n";
	MessageBox(m_hWnd, L"パック名を読み込みました", L"読み込み完了", MB_OK);

}


/**
* @brief 読み込んだ画像設定
* @param [in]カード情報
*/
void My::CEdit::SetLoadTexture(Card& card)
{
	// すでに有効なテクスチャがある場合は何もしない（プレースホルダは除く）
	if (card.ImageTexture && card.ImageTexture != m_PlaceholderTex)
		return;

	// imagePath が空ならプレースホルダを割り当てて終了
	if (card.imagePath.empty())
	{
		card.ImageTexture = m_PlaceholderTex;
		return;
	}

	// フルパス構築（常に "data\\TEXTURE\\" を先頭に付ける）
	std::string fullPath;
	const std::string prefix = "data\\TEXTURE\\";
	if (card.imagePath.rfind(prefix, 0) == 0)
		fullPath = card.imagePath;
	else
		fullPath = prefix + card.imagePath;

	std::cout << "[DEBUG] Loading texture from: " << fullPath << "\n";

	// ファイル存在確認
	std::ifstream ifs(fullPath.c_str(), std::ios::binary);
	if (!ifs.is_open())
	{
		std::cout << "[SetLoadTexture] ファイルが見つかりません: " << fullPath << "\n";
		card.ImageTexture = m_PlaceholderTex;
		return;
	}
	ifs.close();

	// テクスチャ読み込み
	IDirect3DTexture9* newTex = nullptr;
	std::wstring wFull = ConvertToWString(fullPath);
	HRESULT hr = D3DXCreateTextureFromFile(m_pDevice, wFull.c_str(), &newTex);

	if (SUCCEEDED(hr) && newTex)
	{
		// 既存テクスチャがプレースホルダでない場合は解放
		if (card.ImageTexture && card.ImageTexture != m_PlaceholderTex)
		{
			card.ImageTexture->Release();
			card.ImageTexture = nullptr;
		}
		card.ImageTexture = newTex;
	}
	else
	{
		std::cout << "[SetLoadTexture] テクスチャ読み込み失敗: " << fullPath << "\n";
		if (newTex)
		{
			newTex->Release();
			newTex = nullptr;
		}
		card.ImageTexture = m_PlaceholderTex;
	}
}

/**
* @brief 画像設定処理
* @param [in]カード番号
*/
void My::CEdit::SetImage(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
		return;

	Card& card = m_Pack[PackID].cards[ID];

	// 画像選択ボタン
	if (ImGui::Button(u8"画像を選択"))
	{
		const char* filters[] = { "*.png", "*.jpg"};
		std::string path = OPenImageDialog(m_hWnd);

		if (!path.empty())
		{
			std::string fullPath = std::string(path);
			std::string relativePath = TrimTexturePath(fullPath);

			// JSON 保存用に相対パスを保存
			card.imagePath = relativePath;

			// 新しいテクスチャを先に読み込む（失敗したら既存は残す）
			IDirect3DTexture9* newTex = nullptr;
			std::wstring wPath = ConvertToWString(fullPath);
			if (SUCCEEDED(D3DXCreateTextureFromFile(m_pDevice, wPath.c_str(), &newTex)) && newTex)
			{
				// 既存がプレースホルダでないなら安全に解放
				if (card.ImageTexture && card.ImageTexture != m_PlaceholderTex)
				{
					card.ImageTexture->Release();
				}
				// 新しいテクスチャをセット（所有権を得る）
				card.ImageTexture = newTex;
			}
			else
			{
				// 読み込み失敗なら newTex を解放しておく
				if (newTex) { newTex->Release(); newTex = nullptr; }
				// 既存が無ければプレースホルダをセット
				if (!card.ImageTexture)
					card.ImageTexture = m_PlaceholderTex;
			}
		}
	}

	// 画像が未設定の場合、JSONから読み込まれた相対パスを使って読み込む（アプリ起動時や初回表示）
	if (!card.ImageTexture && !card.imagePath.empty())
	{
		std::string potentialPrefix = "data\\TEXTURE\\";
		std::string fullPath;
		if (card.imagePath.size() >= potentialPrefix.size() &&
			card.imagePath.compare(0, potentialPrefix.size(), potentialPrefix) == 0)
		{
			fullPath = card.imagePath;
		}
		else
		{
			fullPath = potentialPrefix + card.imagePath;
		}

		// ファイル存在確認
		std::ifstream ftest(fullPath.c_str());
		if (ftest.is_open())
		{
			ftest.close();
			IDirect3DTexture9* tex = nullptr;
			std::wstring wFullPath = ConvertToWString(fullPath);
			if (SUCCEEDED(D3DXCreateTextureFromFile(m_pDevice, wFullPath.c_str(), &tex)) && tex)
			{
				// 既存がプレースホルダでないなら解放（通常ここは nullptr）
				if (card.ImageTexture && card.ImageTexture != m_PlaceholderTex)
				{
					card.ImageTexture->Release();
				}
				card.ImageTexture = tex;
			}
			else
			{
				if (tex) { tex->Release(); tex = nullptr; }
				if (!card.ImageTexture)
					card.ImageTexture = m_PlaceholderTex;
			}
		}
		else
		{
			// ファイルが存在しない場合はプレースホルダを使う
			if (!card.ImageTexture)
				card.ImageTexture = m_PlaceholderTex;
		}
	}

	// 表示部（ImGui::Image に渡すポインタは常に有効にする）
	IDirect3DTexture9* displayTex = card.ImageTexture ? card.ImageTexture : m_PlaceholderTex;
	if (!displayTex)
	{
		// 最終フォールバック（通常はここに来ない）
		ImGui::Text(u8"画像が設定されていません。");
		if (!card.imagePath.empty())
			ImGui::Text(u8"パス: %s", card.imagePath.c_str());
		return;
	}

	ImGui::Text(u8"選択された画像: %s", card.imagePath.c_str());
	ImGui::Image((void*)displayTex, ImVec2(200, 200));
}

/**
* @brief コスト編集処理
* @param [in]カード番号
*/
void My::CEdit::EditCost(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
		return;

	Card& card = m_Pack[PackID].cards[ID];

	// UI ID をユニークにする（パックとカードを組み合わせ）
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);

	// マイナスボタン
	if (ImGui::Button("-"))
	{
		card.cost = std::max(0, card.cost - 1);
	}

	ImGui::SameLine();

	// コスト表示
	ImGui::Text("%d", card.cost);

	ImGui::SameLine();

	// プラスボタン
	if (ImGui::Button("+"))
	{
		card.cost += 1;
	}

	ImGui::SameLine();
	ImGui::Text(u8"コスト");

	ImGui::PopID();
	ImGui::PopID();

}

/**
* @brief レアリティ編集処理
* @param [in]カード番号
*/
void My::CEdit::EditRarity(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[ID];

	// パックとカードでユニークなIDを作る
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("rarity");

	// 上段
	ImGui::Text(u8"レアリティ");
	ImGui::RadioButton(u8"コモン", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(COMMON));
	ImGui::SameLine();
	ImGui::RadioButton(u8"アンコモン", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(UNCOMMON));
	ImGui::SameLine();
	ImGui::RadioButton(u8"レア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(RARE));
	ImGui::SameLine();
	ImGui::RadioButton(u8"スーパーレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(SUPERRARE));
	ImGui::SameLine();
	ImGui::RadioButton(u8"ハイパーレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(HYPERRARE));
	
	// 下段
	ImGui::RadioButton(u8"ロイヤルレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(ROYALRARE));
	ImGui::SameLine();
	ImGui::RadioButton(u8"エンペラーレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(EMPERORRARE));
	ImGui::SameLine();
	ImGui::RadioButton(u8"ゴッドレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(GODRARE));
	ImGui::SameLine();
	ImGui::RadioButton(u8"エックスレア", reinterpret_cast<int*>(&card.raritytype), static_cast<int>(XRARE));

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();

}

/**
* @brief カードタイプ選択処理
*/
void My::CEdit::SelectType(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[ID];

	// ユニークな ImGui ID を構築
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("type");

	ImGui::Text(u8"カードタイプ");
	ImGui::RadioButton(u8"攻撃", reinterpret_cast<int*>(&card.maintype), static_cast<int>(ATTACK));
	ImGui::RadioButton(u8"守備", reinterpret_cast<int*>(&card.maintype), static_cast<int>(DEFENSE));
	ImGui::RadioButton(u8"アシスト", reinterpret_cast<int*>(&card.maintype), static_cast<int>(ASSIST));

	// 選択によるサブ設定呼び出し（変更があればそれぞれの関数内で処理）
	if (card.maintype == ATTACK)
	{
		SetAttack(PackID, ID);
	}
	else if (card.maintype == DEFENSE)
	{
		SetDefense(PackID, ID);
	}
	else if (card.maintype == ASSIST)
	{
		SetAssist(PackID, ID);
	}

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief アシスト設定処理
* @param [in]カード番号
*/
void My::CEdit::SetAssist(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[ID];

	if (card.heal < 0)
	{// 変な値が入らないように初期化
		card.heal = 0;
	}

	// ユニークな ImGui ID を構築（パック＋カード＋機能ごと）
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("assist");

	ImGui::Text(u8"アシストの種類");

	ImGui::RadioButton(u8"妨害", reinterpret_cast<int*>(&card.assisttype), static_cast<int>(OBSTRUCT));
	ImGui::RadioButton(u8"バフ", reinterpret_cast<int*>(&card.assisttype), static_cast<int>(BUFF));
	ImGui::RadioButton(u8"デバフ", reinterpret_cast<int*>(&card.assisttype), static_cast<int>(DEBUFF));

	// バフが選択されたらバフタイプ表示
	if (card.assisttype == BUFF)
	{
		ImGui::Text(u8"バフの種類");

		ImGui::RadioButton(u8"回復", reinterpret_cast<int*>(&card.bufftype), static_cast<int>(HEAL));
		ImGui::RadioButton(u8"これから実装", reinterpret_cast<int*>(&card.bufftype), static_cast<int>(NO_NAMEONE));
		ImGui::RadioButton(u8"今後実装", reinterpret_cast<int*>(&card.bufftype), static_cast<int>(NO_NAMETWO));

		// 回復バフの詳細編集
		if (card.bufftype == HEAL)
		{
			ImGui::PushID("heal");

			// 回復対象選択
			ImGui::Text(u8"回復対象");
			ImGui::RadioButton(u8"全体回復", reinterpret_cast<int*>(&card.healtype), static_cast<int>(ALL_HEAL));
			ImGui::RadioButton(u8"特定の相手に回復", reinterpret_cast<int*>(&card.healtype), static_cast<int>(SPECIFIC_HEAL));
			ImGui::RadioButton(u8"ランダム回復", reinterpret_cast<int*>(&card.healtype), static_cast<int>(RANDOM_HEAL));
			ImGui::RadioButton(u8"自分を含めてランダム回復", reinterpret_cast<int*>(&card.healtype), static_cast<int>(SELFINTARGET_HEAL));
			ImGui::RadioButton(u8"自分だけ回復", reinterpret_cast<int*>(&card.healtype), static_cast<int>(ONLY_ME));


			if (ImGui::Button("-"))
			{
				card.heal = std::max(0, card.heal - 1);
			}

			ImGui::SameLine();
			ImGui::Text("%d", card.heal);
			ImGui::SameLine();

			if (ImGui::Button("+"))
			{
				card.heal += 1;
			}

			ImGui::SameLine();
			ImGui::Text(u8"回復量");

			ImGui::PopID();
		}
	}
	else if (card.assisttype == DEBUFF)
	{// デバフタイプのとき

	}
	else if (card.assisttype == OBSTRUCT)
	{// 妨害タイプのとき

	}

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief 攻撃設定処理
* @param [in]カード番号
*/
void My::CEdit::SetAttack(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[ID];

	if (card.damage < 0)
	{// 変な値が入らないように初期化
		card.damage = 0;
	}

	// ユニークな ImGui ID を構築
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("attack");

	// 攻撃対象選択
	ImGui::Text(u8"攻撃対象");
	ImGui::RadioButton(u8"全体攻撃", reinterpret_cast<int*>(&card.attacktype), static_cast<int>(ALL_ATTACK));
	ImGui::RadioButton(u8"特定の相手に攻撃", reinterpret_cast<int*>(&card.attacktype), static_cast<int>(SPECIFIC_ATTACK));
	ImGui::RadioButton(u8"ランダム攻撃", reinterpret_cast<int*>(&card.attacktype), static_cast<int>(RANDOM_ATTACK));
	ImGui::RadioButton(u8"自分を含めてランダム攻撃", reinterpret_cast<int*>(&card.attacktype), static_cast<int>(SELFINTARGET_ATTACK));

	// ダメージ数編集
	ImGui::PushID("damage");
	if (ImGui::Button("-"))
	{
		card.damage = std::max(0, card.damage - 1);
	}
	ImGui::SameLine();
	ImGui::Text("%d", card.damage);
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		card.damage += 1;
	}
	ImGui::SameLine();
	ImGui::Text(u8"ダメージ数");
	ImGui::PopID();

	// 攻撃を選択したので他種はリセット
	card.assisttype = NONE_ASSIST;
	card.defensetype = NONE_DEFENSE;

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief 守備設定処理
* @param [in]カード番号
*/
void My::CEdit::SetDefense(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
		return;

	Card& card = m_Pack[PackID].cards[ID];

	if (card.guard < 0)
	{// 変な値が入らないように初期化
		card.guard = 0;
	}

	if (card.counter < 0)
	{// 変な値が入らないように初期化
		card.counter = 0;
	}

	// ユニークな ImGui ID を構築
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("defense");

	// ガード値編集
	ImGui::PushID("guard");
	if (ImGui::Button("-"))
	{
		card.guard = std::max(0, card.guard - 1);
	}
	ImGui::SameLine();
	ImGui::Text("%d", card.guard);
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		card.guard += 1;
	}
	ImGui::SameLine();
	ImGui::Text(u8"ガード値");
	ImGui::PopID(); // guard

	// 反撃の有無選択
	ImGui::Text(u8"反撃の有無");
	ImGui::RadioButton(u8"反撃する", reinterpret_cast<int*>(&card.defensetype), static_cast<int>(COUNTER));
	ImGui::RadioButton(u8"反撃しない", reinterpret_cast<int*>(&card.defensetype), static_cast<int>(NOT_COUNTER));

	// 反撃が有効なら反撃値編集
	if (card.defensetype == COUNTER)
	{
		ImGui::PushID("counter");
		if (ImGui::Button("-"))
		{
			card.counter = std::max(0, card.counter - 1);
		}
		ImGui::SameLine();
		ImGui::Text("%d", card.counter);
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			card.counter += 1;
		}
		ImGui::SameLine();
		ImGui::Text(u8"カウンター値");
		ImGui::PopID(); // counter
	}

	// 守備を選択したため他タイプをリセット
	card.attacktype = NONE_ATTACK;
	card.assisttype = NONE_ASSIST;

	ImGui::PopID(); // defense
	ImGui::PopID(); // ID
	ImGui::PopID(); // PackID
}

/**
* @brief プレビュー画面編集
*/
void My::CEdit::EditPreview()
{
	// プレビュー用子ウィンドウ開始
	ImGuiWindowFlags flags = ImGuiCond_FirstUseEver;
	ImGui::BeginChild("Preview", ImVec2(480, 0), false, flags);

	ImGui::Text(u8"カード画像プレビュー");
	ImGui::Separator();

	// 選択パックチェック
	if (m_SelectPack < 0 || m_SelectPack >= (int)m_Pack.size())
	{
		ImGui::Text(u8"パックを選択してください");
		ImGui::EndChild();
		return;
	}

	auto& cards = m_Pack[m_SelectPack].cards;

	// カード数チェック
	int count = (int)cards.size();
	if (count == 0)
	{
		ImGui::Text(u8"カードがありません");
		ImGui::EndChild();
		return;
	}

	// 選択カード補正
	if (m_Select < 0 || m_Select >= count)
	{
		m_Select = 0;
	}

	// ←→ ナビゲーション
	if (ImGui::ArrowButton("##Prev", ImGuiDir_Left))
		m_Select = (m_Select - 1 + count) % count;
	ImGui::SameLine();
	ImGui::Text("%d / %d", m_Select + 1, count);
	ImGui::SameLine();
	if (ImGui::ArrowButton("##Next", ImGuiDir_Right))
		m_Select = (m_Select + 1) % count;

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	Card& card = cards[m_Select];

	// 選択カード取得直後に必ずテクスチャを初期化（Load 時に設定済みなら何もしない）
	SetLoadTexture(card);

	// 座標の取得
	ImVec2 drawPos = ImGui::GetCursorScreenPos();

	// カード背景設定
	SetBG(m_SelectPack, m_Select, drawPos);

	//カードフレーム設定
	SetCardFrame(m_SelectPack, m_Select, drawPos);

	// カードネームフレーム設定
	SetNameFrame(m_SelectPack,m_Select,drawPos);

	// カードイラストフレーム設定
	SetCardIrustFrame(m_SelectPack, m_Select, drawPos);

	// カードイラスト設定
	SetCardIrust(m_SelectPack, m_Select, drawPos);

	// カードテキストフレーム設定
	SetTxtFrame(m_SelectPack, m_Select, drawPos);

	// タイプフレーム設定
	SetTypeFrame(m_SelectPack, m_Select, drawPos);

	// コストフレーム設定
	SetCostFrame(m_SelectPack, m_Select, drawPos);

	ImGui::EndChild();
}

/**
* @brief デッキ選択画面編集
*/
void My::CEdit::SelectDeckPreview()
{
	if (!m_bWindowSizeDeck)
	{
		ImGui::SetNextWindowSize(ImVec2(300.0f, 300.0f), ImGuiCond_Always);
		m_bWindowSizeDeck = true;
	}

	ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_FirstUseEver);
	ImGui::Begin("Card Pack");

	if (m_SelectPack < 0) m_SelectPack = 0;
	if (m_SelectPack >= (int)m_Pack.size()) m_SelectPack = (int)m_Pack.size() - 1;

	if (ImGui::Button(u8"追加"))
	{
		int nextIndex = (int)m_Pack.size() + 1;
		char buf[128];
		snprintf(buf, sizeof(buf), u8"第%d弾_", nextIndex);
		m_NewPackName = std::string(buf);

		strncpy(m_PackNameBuf, m_NewPackName.c_str(), sizeof(m_PackNameBuf) - 1);
		m_PackNameBuf[sizeof(m_PackNameBuf) - 1] = '\0';
		m_PackRubyBuf[0] = '\0';

		m_ShowNewPackModal = true;
		ImGui::OpenPopup("New Pack");
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"削除") && IsValidPackIndex(m_SelectPack))
	{
		if (m_EditPackIndex == m_SelectPack) m_EditPackIndex = -1;
		RemovePack(m_SelectPack);
		m_SelectPack = CEdit::clamp(m_SelectPack, 0, (int)m_Pack.size() - 1);
		m_Select = -1;
		SavePackName();
	}

	ImGui::Separator();

	for (int i = 0; i < (int)m_Pack.size(); ++i)
	{
		ImGui::PushID(i);
		bool isEditing = (m_EditPackIndex == i);

		std::string displayName = trim_copy(m_Pack[i].Packname);
		if (displayName.empty()) displayName = std::string(u8"無名パック") + std::to_string(i + 1);

		ImGui::BeginGroup();

		if (!isEditing)
		{
			if (ImGui::Selectable(displayName.c_str(), m_SelectPack == i))
			{
				m_SelectPack = i;
			}

			char editLabel[64];
			snprintf(editLabel, sizeof(editLabel), u8"▷編集##edit_%d", i);
			if (ImGui::Button(editLabel))
			{
				m_EditPackIndex = i;
				strncpy(m_PackNameBuf, m_Pack[i].Packname.c_str(), sizeof(m_PackNameBuf) - 1);
				m_PackNameBuf[sizeof(m_PackNameBuf) - 1] = '\0';
				strncpy(m_PackRubyBuf, m_Pack[i].Ruby.c_str(), sizeof(m_PackRubyBuf) - 1);
				m_PackRubyBuf[sizeof(m_PackRubyBuf) - 1] = '\0';
			}
		}
		else
		{
			ImGui::SetNextItemWidth(280);
			char labelName[64];
			snprintf(labelName, sizeof(labelName), "##Packname_edit_%d", i);
			ImGui::InputText(labelName, m_PackNameBuf, sizeof(m_PackNameBuf));

			ImGui::SetNextItemWidth(280);
			char labelRuby[64];
			snprintf(labelRuby, sizeof(labelRuby), "##Packruby_edit_%d", i);
			ImGui::InputText(labelRuby, m_PackRubyBuf, sizeof(m_PackRubyBuf));

			if (ImGui::Button(u8"保存"))
			{
				std::string finalName = trim_copy(std::string(m_PackNameBuf));
				if (finalName.empty()) finalName = std::string(u8"無名パック") + std::to_string(i + 1);
				std::string finalRuby = trim_copy(std::string(m_PackRubyBuf));

				m_Pack[i].Packname = finalName;
				m_Pack[i].Ruby = finalRuby;
				m_EditPackIndex = -1;
				SavePackName();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"キャンセル"))
			{
				m_EditPackIndex = -1;
			}
		}

		ImGui::EndGroup();
		ImGui::PopID();
	}

	if (m_ShowNewPackModal)
	{
		if (ImGui::BeginPopupModal("New Pack", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(u8"パック名を入力してください");
			ImGui::SetNextItemWidth(300);
			ImGui::InputText("##newPackname", m_PackNameBuf, sizeof(m_PackNameBuf));

			ImGui::Text(u8"パックの読み方を入力してください");
			ImGui::SetNextItemWidth(300);
			ImGui::InputText("##newPackruby", m_PackRubyBuf, sizeof(m_PackRubyBuf));

			ImGui::Separator();

			if (ImGui::Button(u8"OK"))
			{
				std::string finalName = trim_copy(std::string(m_PackNameBuf));
				if (finalName.empty())
				{
					int nextIndex = (int)m_Pack.size() + 1;
					char buf2[64];
					snprintf(buf2, sizeof(buf2), u8"第%d弾", nextIndex);
					finalName = std::string(buf2);
				}
				std::string finalRuby = trim_copy(std::string(m_PackRubyBuf));

				CreatePack(finalName, finalRuby);

				m_SelectPack = (int)m_Pack.size() - 1;
				m_Select = -1;
				m_EditPackIndex = -1;

				m_ShowNewPackModal = false;
				ImGui::CloseCurrentPopup();
				m_PackNameBuf[0] = '\0';
				m_PackRubyBuf[0] = '\0';

				SavePackName();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"キャンセル"))
			{
				m_ShowNewPackModal = false;
				ImGui::CloseCurrentPopup();
				m_PackNameBuf[0] = '\0';
				m_PackRubyBuf[0] = '\0';
			}

			ImGui::EndPopup();
		}
		else
		{
			m_ShowNewPackModal = false;
			m_PackNameBuf[0] = '\0';
			m_PackRubyBuf[0] = '\0';
		}
	}

	ImGui::End();

}

/**
* @brief 相対パス抽出処理
* @param [in]画像の絶対パス
* @return 画像の相対パス
*/
std::string My::CEdit::TrimTexturePath(const std::string& fullPath)
{
	const std::string marker = "data\\TEXTURE\\";

	// マーカー位置検索
	size_t pos = fullPath.find(marker);
	if (pos != std::string::npos)
	{
		return fullPath.substr(pos + marker.length()); // 例: "data\\TEXTURE\\image.png"
	}

	// マーカーが見つからない場合はファイル名だけ抽出
	size_t lastSlash = fullPath.find_last_of("\\/");
	if (lastSlash != std::string::npos)
	{
		return "data\\TEXTURE\\" + fullPath.substr(lastSlash + 1);
	}

	return fullPath; // それも見つからなければそのまま
}

/**
* @brief 並べ替え処理
*/
void My::CEdit::Soat()
{
	// pending がセットされていなければ何もしない
	if (m_PendingReorderSrc < 0 || m_PendingReorderDst < 0) return;

	if (!IsValidPackIndex((size_t)m_SelectPack)) { m_PendingReorderSrc = m_PendingReorderDst = -1; return; }
	auto& cards = m_Pack[m_SelectPack].cards;
	int n = (int)cards.size();
	if (n <= 1) { m_PendingReorderSrc = m_PendingReorderDst = -1; return; }

	int src = m_PendingReorderSrc;
	int dst = m_PendingReorderDst;

	// リセット（先にリセットしても良いが安全に後で）
	m_PendingReorderSrc = -1;
	m_PendingReorderDst = -1;

	// 範囲チェック
	if (src < 0 || src >= n || dst < 0 || dst >= n || src == dst) return;

	// 安定移動：std::rotate
	if (src < dst)
		std::rotate(cards.begin() + src, cards.begin() + src + 1, cards.begin() + dst + 1);
	else
		std::rotate(cards.begin() + dst, cards.begin() + src, cards.begin() + src + 1);

	// m_Select 補正
	if (m_Select == src) m_Select = dst;
	else if (src < dst)
	{
		if (m_Select > src && m_Select <= dst) m_Select -= 1;
	}
	else // src > dst
	{
		if (m_Select >= dst && m_Select < src) m_Select += 1;
	}
}

bool My::CEdit::RemovePack(size_t PackIndex)
{
	if (!IsValidPackIndex(PackIndex)) return false;

	// 削除対象パックのカードリソースを解放（プレースホルダは解放しない）
	for (auto& c : m_Pack[PackIndex].cards)
	{
		if (c.ImageTexture && c.ImageTexture != m_PlaceholderTex)
		{
			c.ImageTexture->Release();
			c.ImageTexture = nullptr;
		}
	}

	// パックを削除
	m_Pack.erase(m_Pack.begin() + PackIndex);

	// 選択中インデックス補正
	if (m_Pack.empty())
	{
		m_SelectPack = -1;
		m_Select = -1;
		return true;
	}

	if (m_SelectPack >= (int)m_Pack.size())
	{
		m_SelectPack = (int)m_Pack.size() - 1;
	}

	// 削除したパックが現在選択中だったら選択カードをリセット
	//（安全に -1 にして UI 側で再選択を促す）
	if (m_SelectPack == (int)PackIndex)
	{
		m_Select = -1;
	}
	else
	{
		// m_Select が現在パックのカード数を超えている場合は補正
		if (m_Select >= 0 && m_Select >= (int)m_Pack[m_SelectPack].cards.size())
			m_Select = -1;
	}

	return true;
}

/**
* @brief スケール設定
*/
void My::CEdit::UpdateScale()
{
	ImGuiIO& io = ImGui::GetIO();

	// m_pDevice は LPDIRECT3DDEVICE9
	if (!m_pDevice) return;

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	// 直接保存していないなら PresentParameters を保持する仕組みを追加しておく必要がある
	// ここでは GetBackBuffer からサーフェスを調べる方法で幅高さを取得
	IDirect3DSurface9* pBack = nullptr;
	if (SUCCEEDED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBack)) && pBack)
	{
		D3DSURFACE_DESC desc;
		pBack->GetDesc(&desc);
		pBack->Release();

		float bbw = (float)desc.Width;
		float bbh = (float)desc.Height;

		// ウィンドウクライアントの物理ピクセルサイズ（DPIスケールを考慮して算出する）
		RECT rc; GetClientRect(m_hWnd, &rc);
		float clientW = float(rc.right - rc.left);
		float clientH = float(rc.bottom - rc.top);

		// フレームバッファスケール = バックバッファ / クライアント（物理） の比
		float scaleX = bbw / clientW;
		float scaleY = bbh / clientH;

		io.DisplaySize = ImVec2(clientW, clientH);
		io.DisplayFramebufferScale = ImVec2(scaleX, scaleY);
	}
}

/**
* @brief ファイル設定
* @param [in]ウィンドウハンドル
*/
std::string My::CEdit::OPenImageDialog(HWND hWnd)
{
	std::string result;

	// COM初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	bool comInitialized = SUCCEEDED(hr);

	IFileOpenDialog* pDialog = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));

	if (SUCCEEDED(hr))
	{
		// フィルタ設定（PNGとJPG）
		COMDLG_FILTERSPEC fileTypes[] = 
		{
			{ L"画像ファイル", L"*.png;*.jpg" },
			{ L"すべてのファイル", L"*.*" }
		};

		pDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
		pDialog->SetTitle(L"画像ファイルを選択");

		hr = pDialog->Show(hWnd);
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem = nullptr;
			hr = pDialog->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath = nullptr;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				if (SUCCEEDED(hr))
				{
					// std::wstringからstd::stringに変換
					char pathA[MAX_PATH];
					WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, pathA, MAX_PATH, nullptr, nullptr);
					result = std::string(pathA);
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pDialog->Release();
	}

	if (comInitialized)
	{
		CoUninitialize();
	}
	return result;
}

/**
* @brief カード背景設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetBG(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	const ImVec2 frameSize(CARDFRAME_WIDTH, CARDFRAME_HEIGHT);

	// テクスチャ読み込み（初回のみ）
	if (m_IrustBG == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\bg.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_IrustBG)))
		{
			m_IrustBG = nullptr;
		}
	}

	if (m_IrustBG == nullptr)
	{
		ImGui::Text(u8"背景画像が未ロードです");
		ImGui::Dummy(frameSize);
		return;
	}

	// 描画準備
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// 表示位置の微調整
	const float kShiftRight = ADJUST_APPEAR;
	const float kShiftDown = ADJUST_APPEAR;
	pos.x += kShiftRight;
	pos.y += kShiftDown;

	// カード種別に応じた tint 色
	Card& card = m_Pack[PackID].cards[CardID];
	ImU32 BgColor = IM_COL32(255, 255, 255, 255);
	switch (card.maintype)
	{
	case ATTACK:
		BgColor = IM_COL32(255, 0, 0, 255);   // 赤
		break;
	case DEFENSE:
		BgColor = IM_COL32(0, 0, 255, 255);   // 青
		break;
	case ASSIST:
		BgColor = IM_COL32(255, 0, 255, 255); // ピンク
		break;
	default:
		break;
	}

	// 四隅の座標（ここで傾きや変形も可能）
	ImVec2 p0 = pos; // 左上
	ImVec2 p1 = ImVec2(pos.x + frameSize.x, pos.y); // 右上
	ImVec2 p2 = ImVec2(pos.x + frameSize.x, pos.y + frameSize.y); // 右下
	ImVec2 p3 = ImVec2(pos.x, pos.y + frameSize.y); // 左下

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// ポリゴン描画（四角形）
	dl->AddImageQuad(
		(ImTextureID)m_IrustBG,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		BgColor
	);

	// レイアウト領域の確保
	ImGui::Dummy(frameSize);

}

/**
* @brief カードフレーム設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetCardFrame(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	const ImVec2 frameSize(CARDFRAME_WIDTH, CARDFRAME_HEIGHT);

	// フレームテクスチャ読み込み（初回のみ）
	if (m_CardFrameMask == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\bgtexture.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_CardFrameMask)))
		{
			m_CardFrameMask = nullptr;
		}
	}

	// テクスチャ未ロード時の処理
	if (m_CardFrameMask == nullptr)
	{
		ImGui::Text(u8"フレーム画像が未ロードです");
		ImGui::Dummy(frameSize);
		return;
	}

	// 描画準備
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// 表示位置の微調整
	const float kShiftRight = ADJUST_APPEAR;
	const float kShiftDown = ADJUST_APPEAR + 5.0f;
	pos.x += kShiftRight;
	pos.y += kShiftDown;

	// 四隅の座標（12px/13px マージン付き）
	ImVec2 p0 = ImVec2(pos.x + 12.0f, pos.y + 12.0f); // 左上
	ImVec2 p1 = ImVec2(pos.x + frameSize.x - 13.0f, pos.y + 12.0f); // 右上
	ImVec2 p2 = ImVec2(pos.x + frameSize.x - 13.0f, pos.y + frameSize.y - 13.0f); // 右下
	ImVec2 p3 = ImVec2(pos.x + 12.0f, pos.y + frameSize.y - 13.0f); // 左下

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// tint 色（白＝無色）
	ImU32 tintColor = IM_COL32(255, 255, 255, 255);

	// フレーム描画（ポリゴン）
	dl->AddImageQuad(
		(ImTextureID)m_CardFrameMask,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		tintColor
	);

	// レイアウト領域の確保
	ImGui::Dummy(frameSize);
}

/**
* @brief カードイラスト設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetCardIrust(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	// カード情報
	Card& card = m_Pack[PackID].cards[CardID];
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// テクスチャが読み込まれているかどうか
	if (card.ImageTexture == nullptr)
	{
		ImGui::Text(u8"テクスチャの読み込みに失敗");
		return;
	}

	// UV 切り抜き用
	D3DSURFACE_DESC desc;
	float texW = 1.0f, texH = 1.0f;
	if (SUCCEEDED(card.ImageTexture->GetLevelDesc(0, &desc)))
	{
		texW = static_cast<float>(desc.Width);
		texH = static_cast<float>(desc.Height);
	}

	const float borderPx = 50.0f;
	ImVec2 imgUv0(borderPx / texW, borderPx / texH);
	ImVec2 imgUv1((texW - borderPx) / texW, (texH - borderPx) / texH);

	// UV を 0..1 に制限
	imgUv0.x = ImClamp(imgUv0.x, 0.0f, 1.0f);
	imgUv0.y = ImClamp(imgUv0.y, 0.0f, 1.0f);
	imgUv1.x = ImClamp(imgUv1.x, 0.0f, 1.0f);
	imgUv1.y = ImClamp(imgUv1.y, 0.0f, 1.0f);

	// 表示位置の微調整
	const float kShiftRight = ADJUST_APPEAR;
	const float kShiftDown = ADJUST_APPEAR;
	pos.x += kShiftRight;
	pos.y += kShiftDown;

	// 描画領域（カードの上半分＋パディング）
	float left = pos.x + IRUST_PADDING;
	float right = pos.x + IRUST_SIZE - IRUST_PADDING;
	float top = pos.y + IRUST_PADDING;
	float bottom = pos.y + IRUST_SIZE * 0.5f + 60.0f - IRUST_PADDING;

	ImVec2 p0(left, top);    // 左上
	ImVec2 p1(right, top);    // 右上
	ImVec2 p2(right, bottom); // 右下
	ImVec2 p3(left, bottom); // 左下

	// UV座標（切り抜き）
	ImVec2 uv0(imgUv0.x, imgUv0.y);
	ImVec2 uv1(imgUv1.x, imgUv0.y);
	ImVec2 uv2(imgUv1.x, imgUv1.y);
	ImVec2 uv3(imgUv0.x, imgUv1.y);

	// テクスチャがプレースホルダーの場合
	if (card.ImageTexture == m_PlaceholderTex)
	{
		dl->AddImageQuad(
			(ImTextureID)m_PlaceholderTex,
			p0, p1, p2, p3,
			ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f),
			IM_COL32(255, 255, 255, 255)
		);
	}
	else
	{
		// イラスト描画（ポリゴン）
		dl->AddImageQuad(
			(ImTextureID)card.ImageTexture,
			p0, p1, p2, p3,
			uv0, uv1, uv2, uv3,
			IM_COL32(255, 255, 255, 255)
		);
	}
}

/**
* @brief カードイラストフレーム設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetCardIrustFrame(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	// カード情報
	Card& card = m_Pack[PackID].cards[CardID];
	ImDrawList* dl = ImGui::GetWindowDrawList();
	const ImVec2 frameSize(CARDFRAME_WIDTH, CARDFRAME_HEIGHT);

	// テクスチャ読み込み
	if (m_IllustrationFrame == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\illust.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_IllustrationFrame)))
		{
			m_IllustrationFrame = nullptr;
		}
	}

	if (m_IllustrationFrame == nullptr)
	{
		ImGui::Text(u8"イラストフレーム画像未ロードです");
		ImGui::Dummy(frameSize);
		return;
	}

	// 枠をイラストより少し大きく描画（ポリゴン版）
	float left = pos.x + IRUST_PADDING + 15.0f;
	float right = pos.x + IRUST_SIZE + 20.0f;
	float top = pos.y + IRUST_PADDING + 15.0f;
	float bottom = pos.y + IRUST_SIZE * 0.5f - IRUST_PADDING + 105.0f;

	ImVec2 p0(left, top);    // 左上
	ImVec2 p1(right, top);    // 右上
	ImVec2 p2(right, bottom); // 右下
	ImVec2 p3(left, bottom); // 左下

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// 描画（ポリゴン）
	dl->AddImageQuad(
		(ImTextureID)m_IllustrationFrame,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		IM_COL32(255, 255, 255, 255)
	);

	// レイアウト確保
	ImGui::Dummy(frameSize);
}

/**
* @brief カードネームフレーム設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetNameFrame(int PackID, int CardID, ImVec2 pos)
{
	ImDrawList* dl = ImGui::GetWindowDrawList();
	Card& card = m_Pack[PackID].cards[CardID];

	const ImVec2 frameSize(CARDFRAME_WIDTH, CARDFRAME_HEIGHT);
	const ImVec2 nameFrameSize(NAMEFRAME_WIDTH, NAMEFRAME_HEIGHT);

	// 名前フレーム画像の読み込み（初回のみ）
	if (m_CardNameFrame == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\name.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_CardNameFrame)))
		{
			m_CardNameFrame = nullptr;
		}
	}

	// 名前フレームの位置をカード中央に配置
	const float offsetX = 30.0f;
	const float offsetY = 20.0f;
	ImVec2 nameFramePos = ImVec2(
		pos.x + (frameSize.x - nameFrameSize.x) * 0.5f + offsetX,
		pos.y + (frameSize.y - nameFrameSize.y) * 0.5f + offsetY
	);

	// 四隅の頂点
	ImVec2 p0 = nameFramePos;
	ImVec2 p1 = ImVec2(nameFramePos.x + nameFrameSize.x, nameFramePos.y);
	ImVec2 p2 = ImVec2(nameFramePos.x + nameFrameSize.x, nameFramePos.y + nameFrameSize.y);
	ImVec2 p3 = ImVec2(nameFramePos.x, nameFramePos.y + nameFrameSize.y);

	// UV座標
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// 名前フレーム矩形
	ImRect txtFrameRect(nameFramePos, ImVec2(nameFramePos.x + nameFrameSize.x, nameFramePos.y + nameFrameSize.y));

	// フォント切り替え（大きいフォント）
	if (m_LargeFont)
	{
		ImGui::PushFont(m_LargeFont);
	}

	// テキストサイズと中央揃え位置
	const char* cardName = card.name.c_str();
	ImVec2 textSize = ImGui::CalcTextSize(cardName);
	ImVec2 textPos = ImVec2(
		txtFrameRect.GetCenter().x - textSize.x * 0.5f,
		txtFrameRect.GetCenter().y - textSize.y * 0.5f - 5.0f
	);

	// 名前フレームとテキスト描画
	if (m_CardNameFrame)
	{
		dl->AddImageQuad((ImTextureID)m_CardNameFrame, p0, p1, p2, p3, uv0, uv1, uv2, uv3, IM_COL32(255, 255, 255, 255));
		dl->AddText(textPos, IM_COL32(0, 0, 0, 255), cardName);
	}
	else
	{
		dl->AddRectFilled(p0, p2, IM_COL32(255, 255, 255, 100), 6.0f);
	}

	// フォントを戻す
	if (m_LargeFont)
	{
		ImGui::PopFont();
	}
}

/**
* @brief テキストフレーム設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetTxtFrame(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	// テクスチャ読み込み（初回のみ）
	if (m_CardTxtFrame == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\text.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_CardTxtFrame)))
		{
			m_CardTxtFrame = nullptr;
		}
	}

	// 読み込み失敗時
	if (m_CardTxtFrame == nullptr)
	{
		ImGui::Text(u8"テキストフレーム画像未ロードです");
		return;
	}

	// 描画準備
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// カードサイズ
	const ImVec2 frameSize(CARDFRAME_WIDTH, CARDFRAME_HEIGHT);

	// テキスト枠サイズ
	const ImVec2 txtFrameSize(TXTFRAME_WIDTH, TXTFRAME_HEIGHT);

	// カード下部に配置（中央揃え + 下端から少し上）
	const float offsetX = (frameSize.x - txtFrameSize.x) * 0.5f;
	const float bottomMargin = 4.0f; // ← ここを調整すれば縦位置を下げられる
	const float offsetY = frameSize.y - txtFrameSize.y - bottomMargin;

	ImVec2 txtFramePos = ImVec2(pos.x + offsetX, pos.y + offsetY);

	// 四隅の頂点（マージン調整済み）
	float left = txtFramePos.x + 50.0f;
	float right = txtFramePos.x + txtFrameSize.x + 5.0f;
	float top = txtFramePos.y + 20.0f;
	float bottom = txtFramePos.y + txtFrameSize.y + 20.0f;

	ImVec2 p0(left, top);    // 左上
	ImVec2 p1(right, top);    // 右上
	ImVec2 p2(right, bottom); // 右下
	ImVec2 p3(left, bottom); // 左下

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// 描画（ポリゴン）
	dl->AddImageQuad(
		(ImTextureID)m_CardTxtFrame,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		IM_COL32(255, 255, 255, 255)
	);
}

/**
* @brief タイプフレーム設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetTypeFrame(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	// テクスチャ読み込み（初回のみ）
	if (m_CardTypeFrame == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\type.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_CardTypeFrame)))
		{
			m_CardTypeFrame = nullptr;
		}
	}

	// 読み込み失敗時
	if (m_CardTypeFrame == nullptr)
	{
		ImGui::Text(u8"タイプフレーム画像未ロードです");
		return;
	}

	// 描画準備
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// タイプフレームサイズ
	const ImVec2 TypeFrameSize(TYPEFRAME_WIDTH, TYPEFRAME_HEIGHT);

	// カード種別に応じた tint 色
	Card& card = m_Pack[PackID].cards[CardID];
	ImU32 BgColor = IM_COL32(255, 255, 255, 255);
	switch (card.maintype)
	{
	case ATTACK:  
		BgColor = IM_COL32(255, 0, 0, 255);  
		break;
	case DEFENSE: 
		BgColor = IM_COL32(0, 0, 255, 255);   
		break;
	case ASSIST: 
		BgColor = IM_COL32(255, 0, 255, 255); 
		break;
	default:      
		break;
	}

	// 右方向にずらすオフセット
	const float offsetRight = 223.0f;
	const float offsetDown = 25.0f;

	// タイプフレームの描画位置
	ImVec2 typeFramePos = ImVec2(pos.x + offsetRight, pos.y + offsetDown);

	// 四隅の座標（TypeFrameSize を使う）
	ImVec2 p0 = typeFramePos;
	ImVec2 p1 = ImVec2(typeFramePos.x + TypeFrameSize.x, typeFramePos.y);
	ImVec2 p2 = ImVec2(typeFramePos.x + TypeFrameSize.x, typeFramePos.y + TypeFrameSize.y);
	ImVec2 p3 = ImVec2(typeFramePos.x, typeFramePos.y + TypeFrameSize.y);

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// ポリゴン描画
	dl->AddImageQuad(
		(ImTextureID)m_CardTypeFrame,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		BgColor
	);

	// タイプのアイコンを出す
	SetTypeIcon(PackID, CardID, pos);

	// レイアウト確保（右側に合わせる）
	ImGui::SetCursorScreenPos(typeFramePos);
	ImGui::Dummy(TypeFrameSize);
}

/**
* @brief タイプアイコン設定
* @param [in]パック番号
* @param [in]カード番号
* @param [in]位置
*/
void My::CEdit::SetTypeIcon(int PackID, int CardID, ImVec2 pos)
{
	std::wstring path = {};	// テクスチャパス
	Card& card = m_Pack[PackID].cards[CardID];

	// 毎回テクスチャを更新（Release → 再読み込み）
	if (card.typeIcon != nullptr)
	{
		card.typeIcon->Release();
		card.typeIcon = nullptr;
	}


	if (card.typeIcon == nullptr)
	{
		switch (card.maintype)
		{
		case ATTACK:
			path = L"data\\TEXTURE\\attack.png";	// 読み込むテクスチャ設定

			break;
		case DEFENSE:
			path = L"data\\TEXTURE\\shield.png";	// 読み込むテクスチャ設定

			break;
		case ASSIST:
			path = L"data\\TEXTURE\\assist.png";	// 読み込むテクスチャ設定

			break;
		}

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &card.typeIcon)))
		{
			card.typeIcon = nullptr;
		}

	}

	if (card.typeIcon != nullptr)
	{// アイコンが正しく読み込まれているとき

		ImDrawList* dl = ImGui::GetWindowDrawList();

		const float offsetX = 280.0f;
		const float offsetY = 45.0f;

		ImVec2 adjustPos = ImVec2(pos.x + offsetX, pos.y + offsetY);

		const ImVec2 iconSize(TYPEICON_WIDTH, TYPEICON_HEIGHT);
		const ImVec2 center = ImVec2(adjustPos.x + iconSize.x * 0.5f, adjustPos.y + iconSize.y * 0.5f);

		bool shouldRotate = (card.maintype == ATTACK); // 例：ATTACKタイプだけ回転
		float angle_rad = IM_PI * TURN_QUATER; // 45度回転

		if (shouldRotate)
		{// 回転させるもの(攻撃タイプ)
			ImVec2 local[MAX_VERTEX] = {
				ImVec2(-iconSize.x, -iconSize.y),
				ImVec2(iconSize.x, -iconSize.y),
				ImVec2(iconSize.x,  iconSize.y),
				ImVec2(-iconSize.x,  iconSize.y),
			};

			ImVec2 rotated[MAX_VERTEX];
			for (int i = 0; i < MAX_VERTEX; ++i)
			{
				float x = local[i].x;
				float y = local[i].y;
				rotated[i] = ImVec2(center.x + x * cosf(angle_rad) - y * sinf(angle_rad),center.y + x * sinf(angle_rad) + y * cosf(angle_rad));
			}

			dl->AddImageQuad(
				(ImTextureID)card.typeIcon,
				rotated[0], rotated[1], rotated[2], rotated[3],
				ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1)
			);
		}
		else
		{
			ImVec2 p0 = adjustPos;
			ImVec2 p1 = ImVec2(adjustPos.x + iconSize.x, adjustPos.y);
			ImVec2 p2 = ImVec2(adjustPos.x + iconSize.x, adjustPos.y + iconSize.y);
			ImVec2 p3 = ImVec2(adjustPos.x, adjustPos.y + iconSize.y);

			dl->AddImageQuad(
				(ImTextureID)card.typeIcon,
				p0, p1, p2, p3,
				ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1)
			);
		}
	}

}

/**
* @brief コストフレーム設定
*/
void My::CEdit::SetCostFrame(int PackID, int CardID, ImVec2 pos)
{
	// 安全チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		ImGui::Text(u8"無効なカードID");
		return;
	}

	Card& card = m_Pack[PackID].cards[CardID];

	// テクスチャ読み込み（初回のみ）
	if (m_CardCostFrame == nullptr)
	{
		std::wstring path = ConvertToWString("data\\TEXTURE\\cost.png");
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_CardCostFrame)))
		{
			m_CardCostFrame = nullptr;
		}
	}

	// 読み込み失敗時
	if (m_CardCostFrame == nullptr)
	{
		ImGui::Text(u8"タイプフレーム画像未ロードです");
		return;
	}

	// 描画準備
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// コストフレームサイズ
	const ImVec2 CostFrameSize(COSTFRAME_WIDTH, COSTFRAME_WIDTH);

	ImU32 ImgColor = IM_COL32(255, 255, 255, 255);	// 画像の色設定

	// 四隅の座標（TypeFrameSize を使う）
	ImVec2 p0 = pos;
	ImVec2 p1 = ImVec2(pos.x + CostFrameSize.x, pos.y);
	ImVec2 p2 = ImVec2(pos.x + CostFrameSize.x, pos.y + CostFrameSize.y);
	ImVec2 p3 = ImVec2(pos.x, pos.y + CostFrameSize.y);

	// UV座標（テクスチャ全体）
	ImVec2 uv0(0.0f, 0.0f);
	ImVec2 uv1(1.0f, 0.0f);
	ImVec2 uv2(1.0f, 1.0f);
	ImVec2 uv3(0.0f, 1.0f);

	// ポリゴン描画
	dl->AddImageQuad(
		(ImTextureID)m_CardCostFrame,
		p0, p1, p2, p3,
		uv0, uv1, uv2, uv3,
		ImgColor
	);

	// 数字描画
	// コスト数値を文字列に変換
	std::string costText = std::to_string(card.cost);

	// フォント切り替え（任意）
	if (m_NumberFont)
	{
		ImGui::PushFont(m_NumberFont);
	}

	// テキストサイズと中央揃え位置
	ImVec2 textSize = ImGui::CalcTextSize(costText.c_str());
	ImVec2 textPos = ImVec2(
		pos.x + (CostFrameSize.x - textSize.x) * 0.5f,
		pos.y + (CostFrameSize.y - textSize.y) * 0.5f
	);

	// 数字描画（中央に重ねる）
	dl->AddText(textPos, IM_COL32(0, 0, 0, 255), costText.c_str());

	if (m_NumberFont)
	{
		ImGui::PopFont();
	}
	
	// レイアウト確保（右側に合わせる）
	ImGui::SetCursorScreenPos(pos);
	ImGui::Dummy(CostFrameSize);

}
