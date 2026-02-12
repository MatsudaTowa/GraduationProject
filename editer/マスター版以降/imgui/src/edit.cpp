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
#include "imgui/imgui_internal.h"
#include "tiny_aes/aes.h"
#include <ShObjIdl.h>
#pragma comment(lib,"imm32")

// std::string → std::wstring に変換する関数
std::wstring ConvertToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], len);
	return wstr;
}

/**
* @brief CBOR暗号化
*/
std::vector<uint8_t> AES_Encrypt(const std::vector<uint8_t>& plaintext,
	const std::vector<uint8_t>& key,
	const std::vector<uint8_t>& iv) 
{
	std::vector<uint8_t> buffer = plaintext;

	// PKCS7風パディング（1〜16バイト）
	size_t pad_len = AES_BLOCKLEN - (buffer.size() % AES_BLOCKLEN);
	buffer.insert(buffer.end(), pad_len, static_cast<uint8_t>(pad_len));

	AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key.data(), iv.data());
	AES_CBC_encrypt_buffer(&ctx, buffer.data(), buffer.size());

	return buffer;
}

/**
* @brief CBOR複合化
*/
std::vector<uint8_t> AES_Decrypt(const std::vector<uint8_t>& ciphertext,
	const std::vector<uint8_t>& key,
	const std::vector<uint8_t>& iv)
{
	std::vector<uint8_t> buffer = ciphertext;

	AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key.data(), iv.data());
	AES_CBC_decrypt_buffer(&ctx, buffer.data(), buffer.size());

	// PKCS7風パディング除去
	if (!buffer.empty()) {
		uint8_t pad_len = buffer.back();
		if (pad_len > 0 && pad_len <= AES_BLOCKLEN && buffer.size() >= pad_len) {
			buffer.resize(buffer.size() - pad_len);
		}
	}

	return buffer;
}

// JSON 安全取得ヘルパー
static int json_get_int_safe(const ordered_json& j, const char* key, int def = 0) 
{
	if (j.contains(key) && j[key].is_number_integer()) return j[key].get<int>();
	return def;
}

static float json_get_float_safe(const ordered_json& j, const char* key, float def = 0.0f)
{
	if (j.contains(key) && j[key].is_number())
	{
		return j[key].get<float>();
	}
	return def;
}

static std::string json_get_string_safe(const ordered_json& j, const char* key, const std::string& def = "") 
{
	if (j.contains(key) && j[key].is_string())
	{
		return j[key].get<std::string>();
	}
	return def;
}

static bool json_get_bool_safe(const ordered_json& j, const char* key, bool def = true) 
{
	if (j.contains(key) && j[key].is_boolean())
	{
		return j[key].get<bool>();
	}

	return def;
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
	m_bWindowSizeCard(false),
	m_EditingReferenceIndex(-1),
	m_ForceCentralSelection(false),
	m_RequestJson(false),
	m_SavepackName(false)
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
	if (m_RequestJson)
	{
		Save();

		SaveJson();

		SavePackNameJson();

		if (m_SavepackName)
		{
			SavePackName();
			m_SavepackName = false;
		}

		// フラグのクリア
		m_RequestJson = false;
	}
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

	// 横スクロール機能追加
	ImGui::BeginChild("EditorScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

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
			// 新規カードをパックに追加(初期化)
			cards.push_back(Card{
				u8"名前なし",	// カード名
				u8"名前なし",	// カードの読み方
				0,				// コスト
				0,				// ダメージ数
				0,				// ガード値
				0,				// 反撃値
				0,				// 回復値
				false,			// 効果の発動時間(true = 単発,false = 単発じゃない)
				0,				// 発動時間
				false,			// 参照の有無
				0,				// 変化先のカードのパック番号
				0,				// 変化先のカードのカード番号
				-1,
				CardType::ATTACK,				// カードのメインタイプ
				RARITY::NONE_RARITY,			// カードのレアリティ
				AssistType::NONE_ASSIST,		// アシストの種類
				AttackType::NONE_ATTACK,		// 攻撃の種類
				DefenseType::NONE_DEFENSE,		// 守備の種類
				HealType::NONE_HEAL,			// 回復の種類
				AddEffect::EFFECT_NONE,			// 効果追加の種類
				"",								// 画像ファイルパス
				nullptr,						// テクスチャのポインタ
				}
			);	
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
				m_RequestJson = true;
				m_SavepackName = true;
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

	ImGui::EndChild(); // EditorScroll

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
	NormalizeReference();

	// 共通シリアライズを使う（m_Pack が最新の UI 状態を反映していることが前提）
	ordered_json root = BuildCardsJson();

	// CBOR 変換
	std::vector<uint8_t> cbor_bytes = ordered_json::to_cbor(root);

	// 固定鍵・IV（既存実装と同じ）
	std::vector<uint8_t> key(32, 0x01);
	std::vector<uint8_t> iv(16, 0x02);

	// 暗号化
	std::vector<uint8_t> encrypted = AES_Encrypt(cbor_bytes, key, iv);

	// ファイル保存
	const std::string path = "data\\json\\cards.cbor";
	std::ofstream ofs(path, std::ios::binary);
	if (!ofs.is_open())
	{
		MessageBox(m_hWnd, L"暗号化セーブに失敗しました", L"失敗ログ", MB_OK);
		return;
	}
	ofs.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
	ofs.close();

	MessageBox(m_hWnd, L"暗号化セーブに成功しました!!", L"成功ログ", MB_OK);
}

/**
* @brief パック名保存処理
*/
void My::CEdit::SavePackName()
{
	ordered_json root;
	root[u8"パック"] = ordered_json::array();

	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{
		const auto& pack = m_Pack[packIdx];

		std::string packName = trim_copy(pack.Packname);
		if (packName.empty()) packName = std::string(u8"");

		std::string packNameRuby = trim_copy(pack.Ruby);
		if (packNameRuby.empty()) packNameRuby = std::string(u8"");

		ordered_json pj;
		pj["Pack Name"] = packName;
		pj["Pack Name Ruby"] = packNameRuby;

		root[u8"パック"].push_back(pj);
	}

	// CBOR変換
	std::vector<uint8_t> cbor_bytes = ordered_json::to_cbor(root);

	// 鍵とIV（固定値）
	std::vector<uint8_t> key(32, 0x01);
	std::vector<uint8_t> iv(16, 0x02);

	// AES暗号化（PKCS7風パディング）
	std::vector<uint8_t> encrypted = AES_Encrypt(cbor_bytes, key, iv);

	// ファイル保存（バイナリモード）
	const std::string path = "data\\json\\packs.cbor";
	std::ofstream ofs(path, std::ios::binary);
	if (!ofs.is_open())
	{
		MessageBox(m_hWnd, L"パック名の暗号化セーブに失敗しました", L"失敗ログ", MB_OK);
		return;
	}

	ofs.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
	ofs.close();

	MessageBox(m_hWnd, L"パック名の暗号化セーブに成功しました！", L"成功ログ", MB_OK);
}

/**
* @brief ロード処理
*/
void My::CEdit::Load()
{
	// 読み込むファイル設定
	const std::string relPath = "data\\json\\cards.cbor";

	std::ifstream ifs(relPath, std::ios::binary);

	if (!ifs.is_open())
	{// 開けなかった場合
		MessageBox(m_hWnd, L"暗号化ファイルが存在しません", L"エラー", MB_OK);
		return;
	}

	std::vector<uint8_t> encrypted((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	std::vector<uint8_t> key(32, 0x01);
	std::vector<uint8_t> iv(16, 0x02);

	std::vector<uint8_t> decrypted = AES_Decrypt(encrypted, key, iv);

	ordered_json Json;
	try
	{
		Json = ordered_json::from_cbor(decrypted);
	}
	catch (...)
	{
		MessageBox(m_hWnd, L"CBOR の復号またはパースに失敗しました", L"エラー", MB_OK);
		return;
	}

	std::string cardKey;
	if (Json.contains(u8"カード"))
		cardKey = u8"カード";

	ordered_json cardNode;
	if (!cardKey.empty()) cardNode = Json[cardKey];
	else if (Json.is_array()) cardNode = Json;

	if (cardNode.is_null())
	{
		MessageBox(m_hWnd, L"\"カード\"キーが存在しません", L"エラー", MB_OK);
		return;
	}

	m_Pack.clear();

	auto ProcessCardJson = [this](const ordered_json& j)
	{
		int packID = j.value("Pack ID", 1);
		size_t packIndex = (packID > 0) ? (size_t)(packID - 1) : 0;

		while (m_Pack.size() <= packIndex)
		{
			Pack pack;
			pack.Packname = "Pack" + std::to_string(m_Pack.size() + 1);	// パック名
			m_Pack.push_back(pack);
		}

		Card card;
		card.name = j.value("Card Name", "名前なし");	// カード名
		card.ruby = j.value("Card Name Ruby", "名前なし");	// カードの読み方
		card.imagePath = trim_copy(j.value("image", ""));	// 仕様イラスト
		card.cost = j.value("cost", 0);	// コスト
		card.raritytype = (RARITY)j.value("rarity", 0);	// レアリティ
		card.maintype = (CardType)j.value("type", 0);	// カードの種類
		card.isOneTime = j.value("isOneTime", false);	// 効果が単発かどうか
		card.target = (j.value("target", 0) != 0);

		if (!card.isOneTime)
			card.time = j.value("time", 0);

		switch (card.maintype)
		{
		case ATTACK:
			card.attacktype = (AttackType)j.value("attacktype", 0);
			card.damage = j.value("power", 0);
			break;
		case DEFENSE:
			card.defensetype = (DefenseType)j.value("defensetype", 0);
			card.guard = j.value("guard", 0);
			if (card.defensetype == COUNTER)
				card.counter = j.value("counter", 0);
			break;
		case ASSIST:
			card.assisttype = (AssistType)j.value("assisttype", 0);
			if (card.assisttype == HEAL)
			{
				card.healtype = (HealType)j.value("healtype", 0);
				card.heal = j.value("heal", 0);
			}
			break;
		}

		card.changePackID = j.value("change_target_pack", 0);
		card.changeCardID = j.value("change_target_card", 0);

		// 参照先の設定
		if (j.contains("references") && j["references"].is_array())
		{
			for (const auto& rj : j["references"])
			{
				if (rj.is_object())
				{
					Reference r = Reference::FromJson(rj);
					card.references.push_back(r);
				}
			}
		}

		// 効果追加の設定
		if (j.contains("effects") && j["effects"].is_array())
		{
			for (const auto& ej : j["effects"])
			{
				if (!ej.is_object()) continue;

				EffectEntry ef;
				ef.type = (AddEffect)ej.value("type", 0);

				// エナジー
				if (ej.contains("energy"))
				{
					const auto& en = ej["energy"];
					ef.energy.num = en.value("num", 1);
					ef.energy.member = en.value("member", 1);
					ef.energy.ope = en.value("ope", 0);
					ef.energy.selecttarget = en.value("selecttarget", 0);
				}

				// ダメージ
				if (ej.contains("damage"))
				{
					const auto& dm = ej["damage"];
					ef.damage.num = dm.value("num", 1);
					ef.damage.member = dm.value("member", 1);
					ef.damage.selecttarget = dm.value("selecttarget", 0);
				}

				// ドロー
				if (ej.contains("draw"))
				{
					const auto& dr = ej["draw"];
					ef.draw.num = dr.value("num", 1);
					ef.draw.member = dr.value("member", 1);
					ef.draw.selecttarget = dr.value("selecttarget", 0);
				}

				card.effects.push_back(ef);
			}
		}

		m_Pack[packIndex].cards.push_back(card);
	};

	if (cardNode.is_array())
	{
		for (const auto& j : cardNode)
			if (j.is_object()) ProcessCardJson(j);
	}
	else if (cardNode.is_object())
	{
		for (auto it = cardNode.begin(); it != cardNode.end(); ++it)
			if (it.value().is_object()) ProcessCardJson(it.value());
	}

	// テクスチャ読み込み
	for (auto& pack : m_Pack)
		for (auto& card : pack.cards)
			SetLoadTexture(card);

	MessageBox(m_hWnd, L"カードデータの読み込みに成功しました！", L"読み込み完了", MB_OK);
}


/**
* @brief Json保存処理
*/
void My::CEdit::SaveJson()
{
	NormalizeReference();

	// BuildCardsJson を使って ordered_json を作成（ReassignReferenceLabels は呼ばない）
	ordered_json root = BuildCardsJson();

	// ファイル保存
	const std::string path = "data\\json\\cards.json";
	std::ofstream ofs(path, std::ios::binary);

	if (!ofs.is_open())
	{
		MessageBox(m_hWnd, L"セーブに失敗しました", L"失敗ログ", MB_OK);
		return;
	}

	ofs << root.dump(4);
	ofs.close();

	MessageBox(m_hWnd, L"セーブに成功", L"成功ログ", MB_OK);
}

/**
* @brief パック名Jsonファイル保存
*/
void My::CEdit::SavePackNameJson()
{
	ordered_json root;

	root[u8"パック名"] = ordered_json::array();

	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{// パック数分回す
		const auto& pack = m_Pack[packIdx];

		std::string packName = trim_copy(pack.Packname);	// パック名

		if (packName.empty())
		{// パック名が空だった場合
			packName = std::string(u8"");
		}

		std::string packNameRuby = trim_copy(pack.Ruby);	// パック名の読み方

		if (packNameRuby.empty())
		{// パック名の読み方が空だった場合
			packNameRuby = std::string(u8"");
		}

		ordered_json pj;
		pj["Pack Name"] = packName;	// パック名
		pj["Pack Name Ruby"] = packNameRuby;	// パック名の読み方

		root[u8"パック"].push_back(pj);
	}

	// ファイル保存
	const std::string path = "data\\json\\packs.json";
	std::ofstream ofs(path, std::ios::binary);

	if (!ofs.is_open())
	{// ファイルが開けなかった場合
		MessageBox(m_hWnd, L"セーブに失敗しました", L"失敗ログ", MB_OK);
	}
	else
	{
		ofs << root.dump(4);
		ofs.close();

		MessageBox(m_hWnd, L"セーブに成功しました", L"成功ログ", MB_OK);

	}
}

/**
* @brief Json読み込み処理
*/
void My::CEdit::LoadJson()
{

}

/**
* @brief パック名ロード処理
*/
void My::CEdit::LoadPackName()
{
	const std::string relPath = "data\\json\\packs.cbor";

	std::cout << "Attempting to load encrypted pack names file: " << relPath << "\n";

	std::ifstream ifs(relPath, std::ios::binary);
	if (!ifs.is_open())
	{// ファイルが開けなかった場合
		MessageBox(m_hWnd, L"パック名ファイルが見つかりません", L"読み込み", MB_OK);
		std::cout << "Failed to open: " << relPath << "\n";
		return;
	}

	std::vector<uint8_t> encrypted((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	// 鍵とIV（SavePackNameと同じ値）
	std::vector<uint8_t> key(32, 0x01);
	std::vector<uint8_t> iv(16, 0x02);

	// 復号
	std::vector<uint8_t> decrypted = AES_Decrypt(encrypted, key, iv);

	// CBORからJSONに変換
	ordered_json root;
	try 
	{// 成功した場合
		root = ordered_json::from_cbor(decrypted);
	}
	catch (const std::exception& e) 
	{// 失敗した場合
		MessageBox(m_hWnd, L"パック名ファイルの復号またはパースに失敗しました", L"読み込みエラー", MB_OK);
		std::cout << "CBOR parse error (packs): " << e.what() << "\n";
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
			if (pj.contains("Pack Name") && pj["Pack Name"].is_string())
				name = pj["Pack Name"].get<std::string>();
			else if (pj.contains("PackName") && pj["PackName"].is_string())
				name = pj["PackName"].get<std::string>();
			else if (pj.contains("Pack") && pj["Pack"].is_string())
				name = pj["Pack"].get<std::string>();

			if (pj.contains("Pack Name Ruby") && pj["Pack Name Ruby"].is_string())
				nameRuby = pj["Pack Name Ruby"].get<std::string>();
			else if (pj.contains("PackNameRuby") && pj["PackNameRuby"].is_string())
				nameRuby = pj["PackNameRuby"].get<std::string>();
			else if (pj.contains("Pack") && pj["Pack"].is_string())
				nameRuby = pj["Pack"].get<std::string>();
		}
		else if (pj.is_string())
		{
			name = pj.get<std::string>();
			nameRuby = pj.get<std::string>();
		}

		name = trim_copy(name);
		nameRuby = trim_copy(nameRuby);

		if (name.empty())
			name = std::string(u8"無名パック");

		m_Pack[i].Packname = name;
		m_Pack[i].Ruby = nameRuby;

		std::cout << "Loaded Pack[" << i << "] = " << name << "\n";
	}

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

	m_SelectPack = 0;
	m_Select = -1;
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


	// メインタイプ選択によるサブ設定呼び出し（変更があればそれぞれの関数内で処理）
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
		SetActiveTime(PackID, ID);

		SetAssist(PackID, ID);
	}

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief アシスト設定処理
* @param [in]パック番号
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
	{// 変な値が入らないようにする
		card.heal = 0;
	}

	// ユニークな ImGui ID を構築（パック＋カード＋機能ごと）
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("assist");

	ImGui::Text(u8"アシストの種類");

	ImGui::RadioButton(u8"回復", reinterpret_cast<int*>(&card.assisttype), static_cast<int>(HEAL));
	ImGui::RadioButton(u8"これから実装①",reinterpret_cast<int*>(&card.assisttype), static_cast<int>(NONAME_ONE));
	ImGui::RadioButton(u8"これから実装②", reinterpret_cast<int*>(&card.assisttype), static_cast<int>(NONAME_TWO));

	if (card.assisttype == HEAL)
	{// アシストの種類が回復のとき
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
	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief アシスト対象設定
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetAssistTarget(int packID, int ID)
{
	JudgeObject(packID, ID,0);
}

/**
* @brief 効果発動時間設定
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetActiveTime(int PackID, int ID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)ID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[ID];

	if (card.time < 0)
	{// 変な値が入らないように初期化
		card.time = 0;
	}

	// ユニークな ImGui ID を構築（パック＋カード＋機能ごと）
	ImGui::PushID((int)PackID);
	ImGui::PushID(ID);
	ImGui::PushID("assist");

	ImGui::Text(u8"効果が単発かどうか");

	int mode = card.isOneTime ? 0 : 1;	// フラグ設定

	if (ImGui::RadioButton(u8"単発", mode == 0))
	{
		card.isOneTime = true;
	}

	if (ImGui::RadioButton(u8"単発じゃない", mode == 1))
	{
		card.isOneTime = false;
	}

	if (card.isOneTime == false)
	{
		ImGui::Text(u8"効果の発動時間を設定してください");

		ImGui::InputInt(u8"発動時間", &card.time);
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

	// 参照の有無設定
	ImGui::Text(u8"参照の有無");
	if (ImGui::RadioButton(u8"参照する", card.target))
	{
		card.target = true;
	}
	if (ImGui::RadioButton(u8"参照しない", !card.target))
	{
		card.target = false;
	}

	if (card.target)
	{
		//攻撃カードの詳細設定
		SetAttackTarget(PackID, ID);
	}
	else
	{
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

	}

	// 効果追加用関数
	SetEffect(PackID,ID);

	ImGui::PopID();
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief 攻撃対象設定
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetAttackTarget(int packID, int ID)
{
	// 対象物の判断
	//JudgeObject(packID, ID,0);

	DrawReference(packID, ID);
}

/**
* @brief 守備設定処理
* @param [in]パック番号
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
* @brief 守備対象設定
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetDefenseTarget(int packID, int ID)
{
	JudgeObject(packID, ID,0);
}

/**
* @brief 効果追加設定
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetEffect(int PackID, int CardID)
{
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	Card& card = m_Pack[PackID].cards[CardID];

	ImGui::Text(u8"効果追加");

	// 変な値が入らないように初期化
	if (card.effectenergy.member < 0 || card.effectenergy.num < 0)
	{
		card.effectenergy = {};
	}
	if (card.effectdamage.member < 0 || card.effectdamage.num < 0)
	{
		card.effectdamage = {};
	}
	if (card.effectdraw.member < 0 || card.effectdraw.num < 0)
	{
		card.effectdraw = {};
	}

	// 追加効果一覧
	for (size_t i = 0; i < card.effects.size(); ++i)
	{
		EffectEntry& e = card.effects[i];
		ImGui::PushID((int)i);

		// 効果タイトル
		ImGui::Text(u8"効果 %d", (int)i + 1);

		ImGui::Indent(20.0f);

		// 編集ボタンの見た目の切り替え
		const char* editLabel = (card.editingEffectIndex == (int)i) ? u8"▽ 編集" : u8"▷ 編集";

		// 編集ボタン
		if (ImGui::Button(editLabel))
		{
			if (card.editingEffectIndex == (int)i)
				card.editingEffectIndex = -1;
			else
				card.editingEffectIndex = (int)i;
		}

		ImGui::SameLine();

		// 削除ボタン
		if (ImGui::Button(u8"削除"))
		{
			card.effects.erase(card.effects.begin() + i);

			if (card.editingEffectIndex == (int)i)
				card.editingEffectIndex = -1;
			else if (card.editingEffectIndex > (int)i)
				card.editingEffectIndex--;

			ImGui::Unindent(20.0f);
			ImGui::PopID();
			break;
		}

		// 編集ボタンを押したとき詳細編集
		if (card.editingEffectIndex == (int)i)
		{
			ImGui::Separator();
			ImGui::Text(u8"編集内容");

			// 種類選択
			int t = (int)e.type;
			ImGui::RadioButton(u8"ダメージ", &t, (int)EFFECT_DAMAGE);
			ImGui::RadioButton(u8"エナジー", &t, (int)EFFECT_ENERGY);
			ImGui::RadioButton(u8"ドロー", &t, (int)EFFECT_DRAW);
			e.type = (AddEffect)t;

			if (e.type == EFFECT_DAMAGE)
			{
				ImGui::Text(u8"ターゲットを設定");
				ImGui::RadioButton(u8"自分だけ", (int*)&e.damage.selecttarget, EFFECTDAMAGE::target::TARGET_ONLYME);
				ImGui::RadioButton(u8"特定の相手", (int*)&e.damage.selecttarget, EFFECTDAMAGE::target::TARGET_SPECIFIC);
				ImGui::RadioButton(u8"自分を含めずランダム", (int*)&e.damage.selecttarget, EFFECTDAMAGE::target::TARGET_RANDOM);
				ImGui::RadioButton(u8"自分を含めてランダム", (int*)&e.damage.selecttarget, EFFECTDAMAGE::target::TARGET_SELFIN);
				ImGui::RadioButton(u8"全体", (int*)&e.damage.selecttarget, EFFECTDAMAGE::target::TARGET_ALL);

				if (e.damage.selecttarget == EFFECTDAMAGE::target::TARGET_RANDOM ||
					e.damage.selecttarget == EFFECTDAMAGE::target::TARGET_SELFIN)
				{
					ImGui::PushID("member");
					if (ImGui::Button("-##member")) e.damage.member = std::max(1, e.damage.member - 1);
					ImGui::SameLine(); ImGui::Text("%d", e.damage.member);
					ImGui::SameLine();
					if (ImGui::Button("+##member")) e.damage.member++;
					ImGui::SameLine(); ImGui::Text(u8"人数");
					ImGui::PopID();
				}

				ImGui::PushID("damage");
				if (ImGui::Button("-##damage")) e.damage.num = std::max(1, e.damage.num - 1);
				ImGui::SameLine(); ImGui::Text("%d", e.damage.num);
				ImGui::SameLine();
				if (ImGui::Button("+##damage")) e.damage.num++;
				ImGui::SameLine(); ImGui::Text(u8"ダメージ数");
				ImGui::PopID();
			}

			if (e.type == EFFECT_ENERGY)
			{
				ImGui::Text(u8"ターゲットを設定");
				ImGui::RadioButton(u8"自分だけ", (int*)&e.energy.selecttarget, EFFECTENERGY::target::TARGET_ONLYME);
				ImGui::RadioButton(u8"特定の相手", (int*)&e.energy.selecttarget, EFFECTENERGY::target::TARGET_SPECIFIC);
				ImGui::RadioButton(u8"自分を含めずランダム", (int*)&e.energy.selecttarget, EFFECTENERGY::target::TARGET_RANDOM);
				ImGui::RadioButton(u8"自分を含めてランダム", (int*)&e.energy.selecttarget, EFFECTENERGY::target::TARGET_SELFIN);
				ImGui::RadioButton(u8"全体", (int*)&e.energy.selecttarget, EFFECTENERGY::target::TARGET_ALL);

				if (e.energy.selecttarget == EFFECTENERGY::target::TARGET_RANDOM ||
					e.energy.selecttarget == EFFECTENERGY::target::TARGET_SELFIN)
				{
					ImGui::PushID("member");
					if (ImGui::Button("-")) e.energy.member = std::max(1, e.energy.member - 1);
					ImGui::SameLine(); ImGui::Text("%d", e.energy.member);
					ImGui::SameLine();
					if (ImGui::Button("+")) e.energy.member++;
					ImGui::SameLine(); ImGui::Text(u8"人数");
					ImGui::PopID();
				}

				ImGui::PushID("operator");
				ImGui::Text(u8"演算子");

				auto HighlightIf = [&](int ope)->bool 
				{
					if (e.energy.ope == ope)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.9f, 0.3f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
						return true;
					}
					return false;
				};
				auto PopHighlight = [&](bool pushed) { if (pushed) ImGui::PopStyleColor(3); };

				{ bool p = HighlightIf(0); if (ImGui::Button(u8"+", ImVec2(50, 0))) e.energy.ope = 0; PopHighlight(p); }
				{ bool p = HighlightIf(1); if (ImGui::Button(u8"-", ImVec2(50, 0))) { e.energy.ope = 1; e.energy.num = std::max(1, e.energy.num - 1); } PopHighlight(p); }

				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				if (ImGui::InputInt("##value", &e.energy.num))
					e.energy.num = std::max(1, e.energy.num);

				ImGui::SameLine(); ImGui::Text(u8"数値");

				{ bool p = HighlightIf(2); if (ImGui::Button(u8"×", ImVec2(50, 0))) e.energy.ope = 2; PopHighlight(p); }
				{ bool p = HighlightIf(3); if (ImGui::Button(u8"÷", ImVec2(50, 0))) e.energy.ope = 3; PopHighlight(p); }

				ImGui::PopID();
			}

			if (e.type == EFFECT_DRAW)
			{
				ImGui::Text(u8"ターゲットを設定");
				ImGui::RadioButton(u8"自分だけ", (int*)&e.draw.selecttarget, EFFECTDRAW::target::TARGET_ONLYME);
				ImGui::RadioButton(u8"特定の相手", (int*)&e.draw.selecttarget, EFFECTDRAW::target::TARGET_SPECIFIC);
				ImGui::RadioButton(u8"自分を含めないでランダム", (int*)&e.draw.selecttarget, EFFECTDRAW::target::TARGET_RANDOM);
				ImGui::RadioButton(u8"自分を含めてランダム", (int*)&e.draw.selecttarget, EFFECTDRAW::target::TARGET_SELFIN);
				ImGui::RadioButton(u8"全体", (int*)&e.draw.selecttarget, EFFECTDRAW::target::TARGET_ALL);

				if (e.draw.selecttarget == EFFECTDRAW::target::TARGET_RANDOM ||
					e.draw.selecttarget == EFFECTDRAW::target::TARGET_SELFIN)
				{
					ImGui::PushID("member");
					if (ImGui::Button("-##member")) e.draw.member = std::max(1, e.draw.member - 1);
					ImGui::SameLine(); ImGui::Text("%d", e.draw.member);
					ImGui::SameLine();
					if (ImGui::Button("+##member")) e.draw.member++;
					ImGui::SameLine(); ImGui::Text(u8"人数");
					ImGui::PopID();
				}

				ImGui::PushID("draw");
				if (ImGui::Button("-##num")) e.draw.num = std::max(1, e.draw.num - 1);
				ImGui::SameLine(); ImGui::Text("%d", e.draw.num);
				ImGui::SameLine();
				if (ImGui::Button("+##num")) e.draw.num++;
				ImGui::SameLine(); ImGui::Text(u8"枚数");
				ImGui::PopID();
			}

			ImGui::Separator();
		}

		ImGui::Unindent(20.0f);
		ImGui::PopID();
	}

	// 効果追加ボタン
	if (ImGui::Button(u8"追加", ImVec2(50.0f, 0.0f)))
	{
		card.addeffect = true;

		EffectEntry newE;
		newE.type = card.effecttype;

		if (card.effecttype == EFFECT_ENERGY)
		{
			newE.energy = card.effectenergy;
		}
		else if (card.effecttype == EFFECT_DAMAGE)
		{
			newE.damage = card.effectdamage;
		}
		else if (card.effecttype == EFFECT_DRAW)
		{
			newE.draw = card.effectdraw;
		}

		card.effects.push_back(newE);
	}
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

/**
* @brief 対象物の判断
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::JudgeObject(int PackID, int CardID,int refIndex)
{

}

/**
* @brief ゾーンの判断処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::JudgeZone(int PackID, int CardID,int refIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	// 参照配列を安全に取得
	auto& refs = m_Pack[PackID].cards[CardID].references;
	if (refIndex < 0 || refIndex >= (int)refs.size())
	{
		return;
	}

	// 参照先ごとの zone を操作する
	auto& ref = refs[refIndex];

	// ImGui の ID を参照先ごとにユニークにする
	ImGui::PushID(("zone" + std::to_string(refIndex)).c_str());

	ImGui::Text(u8"参照先ゾーン");

	// ラジオボタンは ref.zone を直接操作する
	if (ImGui::RadioButton(u8"山札", ref.zone == 0))
	{
		ref.zone = 0;
	}
	if (ImGui::RadioButton(u8"墓地", ref.zone == 1))
	{
		ref.zone = 1;
	}
	if (ImGui::RadioButton(u8"待機", ref.zone == 2))
	{
		ref.zone = 2;
	}
	if (ImGui::RadioButton(u8"手札", ref.zone == 3))
	{
		ref.zone = 3;
	}
	if (ImGui::RadioButton(u8"フィールド", ref.zone == 4))
	{
		ref.zone = 4;
	}

	Card& card = m_Pack[PackID].cards[CardID];
	//int prevCardZone = card.zone;
	//card.zone = ref.zone; 

	if (ref.zone == 0)
	{// 山札を選択した場合
		SetDeck(PackID, CardID,refIndex);
	}
	else if (ref.zone == 1)
	{// 墓地ゾーンを選択した場合
		SetCemetery(PackID, CardID,refIndex);
	}
	else if (ref.zone == 2)
	{// 待機ゾーンを選択した倍
		SetWait(PackID, CardID,refIndex);
	}
	else if (ref.zone == 3)
	{// 手札を選択した場合
		SetHand(PackID, CardID,refIndex);
	}
	else if (ref.zone == 4)
	{// フィールドを選択した場合
		SetField(PackID, CardID,refIndex);
	}

	ImGui::PopID();
}

/**
* @brief 山札の設定処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::SetDeck(int PackID, int CardID,int refIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	// 参照配列を取得
	auto& refs = m_Pack[PackID].cards[CardID].references;
	if (refIndex < 0 || refIndex >= (int)refs.size())
	{
		return;
	}

	Reference& ref = refs[refIndex];

	// 参照先番号のもののみ
	ImGui::PushID(refIndex);

	if (ref.num < 0)
	{// 変な値が入らないように初期化
		ref.num = 0;
	}

	if (ref.targetselect == 0)
	{
		// 見出し
		ImGui::PushID("startpos");
		ImGui::Text(u8"参照開始位置");

		if (ImGui::RadioButton(u8"上", ref.startpos == 0))
		{// 上から見る
			ref.startpos = 0;
		}
		if (ImGui::RadioButton(u8"下", ref.startpos == 1))
		{// 下から見る
			ref.startpos = 1;
		}

		ImGui::PopID();

		//*******************************
		// 参照幅
		//*******************************
		ImGui::PushID("searchwidth");

		ImGui::Text(u8"参照幅");

		// 選択肢
		if (ImGui::RadioButton(u8"範囲", ref.searchwidth == 0))
		{// 範囲を決めて見る
			ref.searchwidth = 0;
		}
		if (ImGui::RadioButton(u8"特定", ref.searchwidth == 1))
		{// 特定の1枚を見る
			ref.searchwidth = 1;
		}

		ImGui::PopID();

		//*******************************
		// 枚数設定 
		//*******************************
		ImGui::PushID("num");

		ImGui::Text(u8"枚数設定");

		if (ImGui::Button("-"))
		{// -ボタンが押されたら
			ref.num = std::max(1, ref.num - 1);
		}

		ImGui::SameLine();

		// 数字の表示
		ImGui::Text("%d", ref.num);

		ImGui::SameLine();

		if (ImGui::Button("+"))
		{// +ボタンが押されたら
			ref.num += 1;
		}

		if (ref.searchwidth == 0)
		{// 範囲検索だった場合
			JudgeType(PackID, CardID,refIndex);
		}

		ImGui::PopID();
	}

	ImGui::PopID();
}

/**
* @brief 墓地の設定処理
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::SetCemetery(int PackID, int CardID,int refIndex)
{
	//// 範囲チェック
	//if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	//{
	//	return;
	//}

	//Card& card = m_Pack[PackID].cards[CardID];

	//if (card.targetselect == 0)
	//{// 自分のみという選択肢が選ばれたとき
	//	// カードのタイプ判断
	//	//JudgeType(PackID, CardID);
	//}

}

/**
* @brief 待機の設定処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::SetWait(int PackID, int CardID,int refIndex)
{
	//// 範囲チェック
	//if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	//{
	//	return;
	//}

	//Card& card = m_Pack[PackID].cards[CardID];

	//if (card.targetselect == 0)
	//{// 自分のみという選択肢が選ばれた場合
	//	//JudgeType(PackID, CardID);
	//}

}

/**
* @brief 手札の設定処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::SetHand(int PackID, int CardID,int refIndex)
{
	//// 範囲チェック
	//if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	//{
	//	return;
	//}

	//Card& card = m_Pack[PackID].cards[CardID];

	//if (card.targetselect == 0)
	//{// 自分のみという選択肢が選ばれた場合
	//	//JudgeType(PackID, CardID);
	//}
}

/**
* @brief フィールドの設定処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::SetField(int PackID, int CardID,int refIndex)
{
	//// 範囲チェック
	//if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	//{
	//	return;
	//}

	//Card& card = m_Pack[PackID].cards[CardID];

	//if (card.selecttype == 0)
	//{//自分のみという選択肢が選ばれた場合
	//	SelectType(PackID, CardID);
	//}
}

/**
* @brief タイプの判断
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::JudgeType(int PackID, int CardID,int refIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	// 参照配列を取得
	auto& refs = m_Pack[PackID].cards[CardID].references;
	if (refIndex < 0 || refIndex >= (int)refs.size())
	{
		return;
	}

	Reference& ref = refs[refIndex];

	// 参照先番号のもののみ
	ImGui::PushID(refIndex);

	ImGui::PushID("selecttype");
	ImGui::Text(u8"タイプ制限の有無");

	if (ImGui::RadioButton(u8"すべてのタイプ", ref.selecttype == 0))
	{
		ref.selecttype = 0;
	}
	if (ImGui::RadioButton(u8"特定のタイプ", ref.selecttype == 1))
	{
		ref.selecttype = 1;
	}

	// 詳細設定
	if (ref.selecttype == 1)
	{// 特定のタイプだった場合
		ImGui::PushID("cardtype");
		ImGui::Text(u8"対象とするタイプ");

		if (ImGui::RadioButton(u8"攻撃タイプ", ref.cardtype == 0))
		{
			ref.cardtype = 0;
		}
		if (ImGui::RadioButton(u8"守備タイプ", ref.cardtype == 1))
		{
			ref.cardtype = 1;
		}
		if (ImGui::RadioButton(u8"アシストタイプ", ref.cardtype == 2))
		{
			ref.cardtype = 2;
		}

		ImGui::PopID();
	}

	if (ref.selecttype == 0)
	{// すべてのタイプの場合
		JudgeCost(PackID, CardID,refIndex);
	}
	if (ref.selecttype == 1)
	{// 特定のタイプの場合
		JudgeOriginalValue(PackID, CardID,refIndex);
	}
	
	ImGui::PopID();
	ImGui::PopID();
}

/**
* @brief カードの固有の値か判断
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::JudgeOriginalValue(int PackID, int CardID,int refIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID)) 
	{ 
		return; 
	} 

	// 参照配列を取得 
	auto& refs = m_Pack[PackID].cards[CardID].references; 

	if (refIndex < 0 || refIndex >= (int)refs.size()) 
	{ 
		return; 
	} 

	Reference& ref = refs[refIndex];

	ImGui::PushID(refIndex); 
	
	if (ref.cardtype == 2) 
	{// アシストは固有の値がないから無条件でコスト判断 
		ref.judgeoriginalvalue = 1; 
		JudgeCost(PackID, CardID, refIndex); 
	} 
	else 
	{// カードタイプがアシスト以外のとき

		ImGui::PushID("judgeoriginalvalue"); 
		ImGui::Text(u8"固有の値追加"); 

		if (ImGui::RadioButton(u8"追加する", ref.judgeoriginalvalue == 0))
		{
			ref.judgeoriginalvalue = 0; 
		} 
		if (ImGui::RadioButton(u8"追加しない", ref.judgeoriginalvalue == 1))
		{ 
			ref.judgeoriginalvalue = 1; 
			JudgeCost(PackID, CardID, refIndex); 
		} 

		if (ref.judgeoriginalvalue == 0) 
		{ 
			ImGui::PushID("originalvalue");
			if (ref.cardtype == 0) 
			{// 攻撃タイプを選んだとき
				ref.Attackoriginalvalue = 0;	// 固有の値が攻撃値しかないためボタン無し
			}
			else if (ref.cardtype == 1) 
			{// 守備タイプを選んだとき
				if (ImGui::RadioButton(u8"守備値", ref.Defenseoriginalvalue == 0))
				{
					ref.Defenseoriginalvalue = 0; 
				} 
				if (ImGui::RadioButton(u8"カウンター値", ref.Defenseoriginalvalue == 1)) 
				{ 
					ref.Defenseoriginalvalue = 1;
				} 
			} 

			ImGui::PopID(); 

		} 

		ImGui::PopID(); 

	} 

	ImGui::PopID(); 
	
}

/**
* @brief コストの判断
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照先番号
*/
void My::CEdit::JudgeCost(int PackID, int CardID,int refIndex)
{
	//// 範囲チェック
	//if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	//{
	//	return;
	//}

	//Card& card = m_Pack[PackID].cards[CardID];

	//if (card.refcost <= 0)
	//{// 変な値が入らないように初期化
	//	card.refcost = 0;
	//}

	//ImGui::PushID("costjudge");

	//if (card.selecttype == 0)
	//{// カードのタイプをすべてにした場合無条件でコスト条件を設定
	//	card.addcostcondition = 0;
	//}
	//else if (card.selecttype == 1)
	//{// カードのタイプを特定にした場合

	//	ImGui::PushID("addcostcondition");

	//	ImGui::Text(u8"コスト条件");
	//	if (ImGui::RadioButton(u8"追加する", card.addcostcondition == 0))
	//	{
	//		card.addcostcondition = 0;
	//	}
	//	if (ImGui::RadioButton(u8"追加しない", card.addcostcondition == 1))
	//	{
	//		card.addcostcondition = 1;
	//	}

	//	ImGui::PopID();
	//}

	//if (card.addcostcondition == 0)
	//{// コスト条件を追加するを選択した場合
	//	ImGui::PushID("costcondition");

	//	ImGui::Text(u8"コスト条件");

	//	if (ImGui::RadioButton(u8"以上", card.costcondition == 0))
	//	{
	//		card.costcondition = 0;
	//	}
	//	if (ImGui::RadioButton(u8"以下", card.costcondition == 1))
	//	{
	//		card.costcondition = 1;
	//	}
	//	if (ImGui::RadioButton(u8"未満", card.costcondition == 2))
	//	{
	//		card.costcondition = 2;
	//	}
	//	if (ImGui::RadioButton(u8"それより上", card.costcondition == 3))
	//	{
	//		card.costcondition = 3;
	//	}
	//	if (ImGui::RadioButton(u8"等しい", card.costcondition == 4))
	//	{
	//		card.costcondition = 4;
	//	}

	//	ImGui::Text(u8"コスト指定");

	//	if (ImGui::Button("-"))
	//	{
	//		card.refcost = std::max(0, card.refcost - 1);
	//	}
	//	ImGui::SameLine();
	//	ImGui::Text("%d", card.refcost);
	//	ImGui::SameLine();
	//	if (ImGui::Button("+"))
	//	{
	//		card.refcost += 1;
	//	}
	//	ImGui::SameLine();
	//	ImGui::Text(u8"コスト値");


	//	ImGui::PopID();

	//}
	//ImGui::PopID();
}

/**
* @brief 参照追加処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]参照の詳細
*/
void My::CEdit::AddReference(int PackID, int CardID, int Kind)
{
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	Reference r{};
	r.kind = Kind;
	r.label = std::string(u8"参照先");
	r.targetselect = 0;
	r.othertargetselect = 0;
	r.targetobject = 0;
	r.zone = 0;
	r.startpos = 0;
	r.searchwidth = 0;
	r.selecttype = 0;
	r.cardtype = 0;
	r.addcostcondition = 0;
	r.judgeoriginalvalue = 0;
	r.Attackoriginalvalue = 0;
	r.Defenseoriginalvalue = 0;
	r.num = 0;
	r.realValue = 0.0f;
	r.judgeKind = false;
	r.referencenum = 0;
	r.operetorbutton = -1;	// 演算子ボタン(-1は未選択)
	r.showInput = false;

	m_Pack[PackID].cards[CardID].references.push_back(r);
}

/**
* @brief 参照削除処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]削除番号
*/
void My::CEdit::RemoveReference(int PackID, int CardID, int RefIndex)
{
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
		return;

	auto& refs = m_Pack[PackID].cards[CardID].references;
	if (RefIndex < 0 || RefIndex >= (int)refs.size())
		return;

	// 削除
	refs.erase(refs.begin() + RefIndex);

	// m_EditingReferenceIndex の補正
	if (refs.empty())
	{
		m_EditingReferenceIndex = -1;
		m_ForceCentralSelection = false;
	}
	else
	{
		if (m_EditingReferenceIndex >= (int)refs.size())
			m_EditingReferenceIndex = (int)refs.size() - 1;
		if (m_EditingReferenceIndex < 0)
			m_EditingReferenceIndex = 0;
	}

	// m_OpInputRefIndex の補正
	if (m_OpInputRefIndex >= (int)refs.size())
		m_OpInputRefIndex = (int)refs.size() - 1;
	if (m_OpInputRefIndex < 0 && !refs.empty())
		m_OpInputRefIndex = 0;

	// m_OpInputIndex / m_OpImputValue の整合性（存在する参照に合わせる）
	if (m_OpInputRefIndex >= 0 && m_OpInputRefIndex < (int)refs.size())
	{
		m_OpImputValue = static_cast<int>(refs[m_OpInputRefIndex].realValue);
		m_OpInputIntBackUp = m_OpImputValue;
	}
	else
	{
		m_OpImputValue = 0;
		m_OpInputIntBackUp = 0;
	}

	// 参照の showInput フラグや選択状態が残っている場合は安全に初期化
	for (auto& r : refs)
	{
		if (r.operetorbutton < 0)
		{
			r.showInput = false;
		}
	}
}

/**
* @brief 要素移動処理
* @param [in]パック番号
* @param [in]カード番号
* @param [in]移動元
* @param [in]移動先
*/
void My::CEdit::MoveReference(int PackID, int CardID, int srcIndex, int dstIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	auto& refs = m_Pack[PackID].cards[CardID].references;
	
	if (srcIndex < 0 || srcIndex >= (int)refs.size())
	{
		return;
	}

	if (dstIndex < 0)
	{
		dstIndex = 0;
	}

	if (dstIndex >= (int)refs.size())
	{
		dstIndex = (int)refs.size() - 1;
	}

	if (srcIndex == dstIndex)
	{
		return;
	}

	Reference tmp = refs[srcIndex];
	refs.erase(refs.begin() + srcIndex);
	refs.insert(refs.begin() + dstIndex, tmp);

	ReassignReferenceLabels(PackID, CardID);

	if (m_EditingReferenceIndex == srcIndex)
	{
		m_EditingReferenceIndex = dstIndex;
	}
	else if (m_EditingReferenceIndex > srcIndex && m_EditingReferenceIndex <= dstIndex)
	{
		--m_EditingReferenceIndex;
	}
	else if (m_EditingReferenceIndex < srcIndex && m_EditingReferenceIndex >= dstIndex)
	{
		++m_EditingReferenceIndex;
	}
}

/**
* @brief 指定位置に挿入
* @param [in]パック番号
* @param [in]カード番号
* @param [in]挿入先番号
*/
void My::CEdit::MoveLastTo(int PackID, int CardID, int targetIndex)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	auto& refs = m_Pack[PackID].cards[CardID].references;
	if (refs.empty())
	{
		return;
	}

	int last = (int)refs.size() - 1;
	if (targetIndex < 0)
	{
		targetIndex = 0;
	}
	if (targetIndex > last)
	{
		targetIndex = last;
	}
	if (last == targetIndex)
	{
		return;
	}

	Reference tmp = refs[last];
	refs.erase(refs.begin() + last);
	refs.insert(refs.begin() + targetIndex, tmp);
	ReassignReferenceLabels(PackID, CardID);
}

/**
* @brief 描画処理
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::DrawReference(int PackID, int CardID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
	{
		return;
	}

	auto& card = m_Pack[PackID].cards[CardID];
	auto& refs = card.references;

	// 最低1つは参照を確保
	if (refs.empty())
	{
		AddReference(PackID, CardID, 0);
	}

	auto& refsNow = m_Pack[PackID].cards[CardID].references;

	// 編集対象インデックスの整合性
	if (m_EditingReferenceIndex < 0 || m_EditingReferenceIndex >= (int)refsNow.size())
	{
		m_EditingReferenceIndex = 0;
	}

	// --- 左列基準矩形 ---
	ImVec2 headerRectMin(0, 0), headerRectSize(0, 0);
	ImVec2 lastRadioMin(0, 0), lastRadioSize(0, 0);

	if (!refsNow.empty())
	{
		auto& leftRef = refsNow[0];

		ImGui::Text(u8"参照先1");
		headerRectMin = ImGui::GetItemRectMin();
		headerRectSize = ImGui::GetItemRectSize();

		ImGui::Text(u8"対象");
		ImGui::RadioButton(u8"自分##left", &leftRef.targetselect, 0);
		ImGui::RadioButton(u8"自分以外##left", &leftRef.targetselect, 1);
		ImGui::RadioButton(u8"自分を含めた誰か##left", &leftRef.targetselect, 2);

		lastRadioMin = ImGui::GetItemRectMin();
		lastRadioSize = ImGui::GetItemRectSize();

		ImGui::Spacing();
		ImGui::Text(u8"対象物");
		ImGui::RadioButton(u8"ゾーン##left_obj", &leftRef.targetobject, 0);
		ImGui::RadioButton(u8"エナジー##left_obj", &leftRef.targetobject, 1);
		ImGui::RadioButton(u8"HP##left_obj", &leftRef.targetobject, 2);
		ImGui::RadioButton(u8"残り時間##left_obj", &leftRef.targetobject, 3);

		if (leftRef.targetobject == 0)
		{
			int prev = m_EditingReferenceIndex;
			m_EditingReferenceIndex = 0;
			JudgeZone(PackID, CardID, 0);
			m_EditingReferenceIndex = prev;
		}
	}
	else
	{
		headerRectMin = ImGui::GetCursorScreenPos();
		headerRectSize = ImVec2(0, ImGui::GetTextLineHeight());
		lastRadioMin = headerRectMin;
		lastRadioSize = headerRectSize;
	}

	// --- ボタン群パラメータ ---
	const char* opLabels[8] =
	{
		u8"＋(詳細設定)", u8"－(詳細設定)", u8"×(詳細設定)", u8"÷(詳細設定)",
		u8"＋(実数値)",   u8"－(実数値)",   u8"×(実数値)",   u8"÷(実数値)"
	};

	const float OPERATOR_OFFSET = OPERATOR_POSX;
	const float btnW = 120.0f;
	const float btnGap = 4.0f;
	float       btnH = ImGui::GetFrameHeight();

	float leftColumnW = headerRectSize.x;
	if (leftColumnW < btnW) leftColumnW = btnW;

	float baseX = headerRectMin.x + leftColumnW + OPERATOR_OFFSET;
	float baseY = lastRadioMin.y + (lastRadioSize.y - btnH) * 0.5f;

	ImVec2 prevCursorLocal = ImGui::GetCursorPos();

	auto SafeSetCursorPos = [&](float localX, float localY)
	{
		ImGui::SetCursorPos(ImVec2(localX, localY));
		ImGui::Dummy(ImVec2(1.0f, 1.0f));
		ImGui::SetCursorPos(ImVec2(localX, localY));
	};

	bool requestAddPending = false;
	int  pendingPressedIndex = -1;
	bool pendingPressedIsReal = false;
	int  pendingFromRefIndex = -1;

	const ImVec4 detailSelectedCol = ImVec4(0.18f, 0.6f, 0.18f, 1.0f);
	const ImVec4 detailSelectedColHover = ImVec4(0.22f, 0.7f, 0.22f, 1.0f);
	const ImVec4 detailSelectedColActive = ImVec4(0.15f, 0.5f, 0.15f, 1.0f);

	// --- ApplyOperatorToTarget（operetorbutton を使う仕様） ---
	auto ApplyOperatorToTarget = [&](int fromIndex, int targetIndex, int opIndex, bool isReal)
	{
		if (targetIndex >= 0 && targetIndex < (int)refsNow.size())
		{
			Reference& tgt = refsNow[targetIndex];

			if (isReal)
			{
				// 実数側は 4..7 にマップ
				tgt.operetorbutton = opIndex + 4;
				tgt.activeDetailExplicit = true;
				tgt.showInput = true;

				// 入力用変数をセット
				m_OpInputIndex = opIndex;
				m_OpInputRefIndex = targetIndex;
				m_OpImputValue = static_cast<int>(tgt.realValue);
				m_OpInputIntBackUp = m_OpImputValue;
			}
			else
			{
				// 詳細側は 0..3
				tgt.operetorbutton = opIndex;
				tgt.activeDetailExplicit = true;
				tgt.showInput = false;
			}
		}
		else
		{
			// 右隣が無い → 追加保留
			requestAddPending = true;
			pendingPressedIndex = opIndex;
			pendingPressedIsReal = isReal;
			pendingFromRefIndex = fromIndex;
		}
	};

	// ヘルパー: ボタンが「選択状態（緑）」かどうかを判定する共通ロジック
	auto IsOpButtonSelectedVisual = [&](int fromIdx, int targetIdx, int opButtonIndex, bool isReal) -> bool
	{
		// 1) 既存ターゲットがあり、その operetorbutton が一致する場合は選択
		if (targetIdx >= 0 && targetIdx < (int)refsNow.size())
		{
			const Reference& tgt = refsNow[targetIdx];
			if (tgt.operetorbutton == (isReal ? opButtonIndex + 4 : opButtonIndex))
				return true;
		}

		// 2) 追加保留中で、保留内容がこのギャップ(fromIdx) と一致する場合は選択（クリック直後の視覚フィードバック）
		if (requestAddPending && pendingFromRefIndex == fromIdx)
		{
			// pendingPressedIndex は opIndex（0..3）で保存されている
			if (pendingPressedIsReal == isReal && pendingPressedIndex == opButtonIndex)
				return true;
		}

		return false;
	};

	// --- 中央（参照先1 の右側）ボタン群（従来の見た目を崩さず、中央は編集参照基準ではなく「参照1の右隣」固定） ---
	{
		int fromIndex = 0;
		int centralTarget = 1; // 参照先1の右隣（R1）

		for (int i = 0; i < 8; ++i)
		{
			float y = baseY + (btnH + btnGap) * i;

			ImVec2 winPos = ImGui::GetWindowPos();
			float  scrollX = ImGui::GetScrollX();
			float  scrollY = ImGui::GetScrollY();
			float  localX = baseX - winPos.x + scrollX;
			float  localY = y - winPos.y + scrollY;

			SafeSetCursorPos(localX, localY);
			ImGui::PushID(1000 + i);

			const char* label = opLabels[i];

			// isReal 判定と opIndex（0..3）
			bool isReal = (i >= 4);
			int opIndex = isReal ? (i - 4) : i;

			bool isSelected = IsOpButtonSelectedVisual(fromIndex, centralTarget, opIndex, isReal);

			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, detailSelectedCol);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, detailSelectedColHover);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, detailSelectedColActive);
			}

			if (ImGui::Button(label, ImVec2(btnW, 0)))
			{
				if (centralTarget < (int)refsNow.size())
				{
					Reference& targetRef = refsNow[centralTarget];
					if (targetRef.operetorbutton == i)
					{
						targetRef.operetorbutton = -1;
						targetRef.activeDetailExplicit = false;
						targetRef.showInput = false;
					}
					else
					{
						ApplyOperatorToTarget(fromIndex, centralTarget, opIndex, isReal);
					}
				}
				else
				{
					ApplyOperatorToTarget(fromIndex, centralTarget, opIndex, isReal);
				}
			}

			if (isSelected) ImGui::PopStyleColor(3);

			ImGui::PopID();
		}
	}

	// --- 右側: 参照先2以降 ---
	{
		auto& refsNow2 = m_Pack[PackID].cards[CardID].references;

		const float innerGap = 8.0f;
		const float panelW = btnW;
		const float opInnerGap = 50.0f;

		float topY = headerRectMin.y;
		float firstRightX = baseX + btnW + innerGap;
		float nextColX = firstRightX;

		for (int refIndex = 1; refIndex < (int)refsNow2.size(); ++refIndex)
		{
			auto& editRef = refsNow2[refIndex];
			float colX = nextColX;

			ImVec2 winPos = ImGui::GetWindowPos();
			float scrollX = ImGui::GetScrollX();
			float scrollY = ImGui::GetScrollY();
			float localColX = colX - winPos.x + scrollX;
			float localTopY = topY - winPos.y + scrollY;

			SafeSetCursorPos(localColX, localTopY);

			ImGui::PushID(3000 + refIndex);
			ImGui::BeginGroup();

			std::string header2 = std::string(u8"参照先") + std::to_string(refIndex + 1);
			ImGui::Text("%s", header2.c_str());

			ImGui::SameLine();
			ImGui::PushID(4000 + refIndex);
			if (ImGui::Button(u8"削除", ImVec2(60.0f, 0)))
			{
				if ((int)refsNow2.size() > 1)
				{
					RemoveReference(PackID, CardID, refIndex);
					refIndex = std::max(0, refIndex - 1);

					nextColX = firstRightX;
					for (int i = 1; i <= refIndex; ++i)
					{
						float opsXtmp = nextColX + panelW + opInnerGap;
						float opsRightTmp = opsXtmp + btnW;
						nextColX = opsRightTmp + innerGap;
					}

					ImGui::PopID();
					ImGui::EndGroup();
					ImGui::PopID();
					continue;
				}
			}
			ImGui::PopID();

			// 表示条件: operetorbutton が 0..3 のときは詳細UIを表示、
			// 4..7 のときは実数入力（showInput）を表示
			if (editRef.operetorbutton >= 0 && editRef.operetorbutton <= 3)
			{
				// 詳細設定モード
				ImGui::Text(u8"対象");
				ImGui::RadioButton(u8"自分##right", &editRef.targetselect, 0);
				ImGui::RadioButton(u8"自分以外##right", &editRef.targetselect, 1);
				ImGui::RadioButton(u8"自分を含めた誰か##right", &editRef.targetselect, 2);

				ImGui::Spacing();
				ImGui::Text(u8"対象物");
				ImGui::RadioButton(u8"ゾーン##right", &editRef.targetobject, 0);
				ImGui::RadioButton(u8"エナジー##right", &editRef.targetobject, 1);
				ImGui::RadioButton(u8"HP##right", &editRef.targetobject, 2);
				ImGui::RadioButton(u8"残り時間##right", &editRef.targetobject, 3);

				if (editRef.targetobject == 0)
				{
					JudgeZone(PackID, CardID, refIndex);
				}
			}
			else if (editRef.showInput || (editRef.operetorbutton >= 4 && editRef.operetorbutton <= 7))
			{
				// 実数入力モード
				if (m_OpInputRefIndex != refIndex)
				{
					m_OpInputRefIndex = refIndex;
					if (m_OpInputRefIndex >= 0 && m_OpInputRefIndex < (int)refsNow2.size())
					{
						m_OpImputValue = static_cast<int>(refsNow2[m_OpInputRefIndex].realValue);
						m_OpInputIntBackUp = m_OpImputValue;
					}
				}

				ImGui::Spacing();
				ImGui::Text(u8"値");
				ImGui::PushItemWidth(150.0f);
				std::string inputId = std::string("##op_input_right_") + std::to_string(refIndex);
				if (ImGui::InputInt(inputId.c_str(), &m_OpImputValue, 1, 10))
				{
					refsNow2[m_OpInputRefIndex].realValue = static_cast<float>(m_OpImputValue);
				}
				ImGui::PopItemWidth();
			}
			else
			{
				ImGui::Text(u8"操作を選択してください");
			}

			ImGui::EndGroup();

			// --- 各列の右側に +-*/ ボタン群を表示（改良版：右隣が無くてもボタンを描画） ---
			{
				float opsX = colX + panelW + opInnerGap;
				float opsBaseY = baseY;

				for (int bi = 0; bi < 8; ++bi)
				{
					float by = opsBaseY + (btnH + btnGap) * bi;

					ImVec2 winPos2 = ImGui::GetWindowPos();
					float scrollX2 = ImGui::GetScrollX();
					float scrollY2 = ImGui::GetScrollY();
					float localOpsX = opsX - winPos2.x + scrollX2;
					float localBy = by - winPos2.y + scrollY2;

					SafeSetCursorPos(localOpsX, localBy);

					ImGui::PushID(10000 + refIndex * 10 + bi);

					int targetIdx = refIndex + 1;
					bool targetExists = (targetIdx < (int)refsNow2.size());

					// isReal 判定と opIndex（0..3）
					bool isReal = (bi >= 4);
					int opIndex = isReal ? (bi - 4) : bi;

					// 選択状態はターゲットが存在する場合の operetorbutton と、
					// 追加保留中の pending と一致する場合を含める（クリック直後に緑表示）
					bool isSelected = IsOpButtonSelectedVisual(refIndex, targetIdx, opIndex, isReal);

					if (isSelected)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, detailSelectedCol);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, detailSelectedColHover);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, detailSelectedColActive);
					}

					// ボタンは常に描画する（右隣が無くても表示）
					if (ImGui::Button(opLabels[bi], ImVec2(btnW, 0)))
					{
						// 押下時の動作：
						// - 右隣が存在するならトグル/設定を行う
						// - 右隣が存在しないなら ApplyOperatorToTarget を呼んで追加保留させる
						if (targetExists)
						{
							Reference& targetRef = refsNow2[targetIdx];
							if (targetRef.operetorbutton == bi)
							{
								// 既に同じ演算子が設定されている -> 解除
								targetRef.operetorbutton = -1;
								targetRef.activeDetailExplicit = false;
								targetRef.showInput = false;
							}
							else
							{
								// 既存ターゲットに対して設定（詳細 or 実数）
								ApplyOperatorToTarget(refIndex, targetIdx, opIndex, isReal);
							}
						}
						else
						{
							// 右隣が無い -> 追加保留（ApplyOperatorToTarget が requestAddPending をセットする）
							ApplyOperatorToTarget(refIndex, targetIdx, opIndex, isReal);
						}
					}

					if (isSelected) ImGui::PopStyleColor(3);

					ImGui::PopID();
				}

				float opsRight = opsX + btnW;
				nextColX = opsRight + innerGap;
			}

			ImGui::PopID(); // 3000 + refIndex の Pop
		}
	}

	// --- ループ後: 保留していた追加要求が「末尾の隣接ボタン」かを判定して実行 ---
	if (requestAddPending)
	{
		auto& refsNowAfter = m_Pack[PackID].cards[CardID].references;

		if (pendingFromRefIndex >= 0)
		{
			int expectedRightIndex = pendingFromRefIndex + 1;
			if (expectedRightIndex == (int)refsNowAfter.size())
			{
				AddReference(PackID, CardID, 0);

				auto& refsAfter = m_Pack[PackID].cards[CardID].references;
				int newIndex = (int)refsAfter.size() - 1;

				if (pendingPressedIsReal)
				{
					// 新規参照に対して実数値演算子を設定（operetorbutton 方式）
					refsAfter[newIndex].operetorbutton = pendingPressedIndex + 4;
					refsAfter[newIndex].activeDetailExplicit = true;
					refsAfter[newIndex].showInput = true;

					m_OpInputIndex = pendingPressedIndex;
					m_OpInputRefIndex = newIndex;

					if (m_OpInputRefIndex >= 0 && m_OpInputRefIndex < (int)refsAfter.size())
					{
						m_OpImputValue = static_cast<int>(refsAfter[m_OpInputRefIndex].realValue);
						m_OpInputIntBackUp = m_OpImputValue;
					}

					m_EditingReferenceIndex = newIndex;
					m_ForceCentralSelection = false;
				}
				else
				{
					// 新規参照に対して詳細演算子を設定（operetorbutton 方式）
					refsAfter[newIndex].operetorbutton = pendingPressedIndex;
					refsAfter[newIndex].activeDetailExplicit = true;
					refsAfter[newIndex].showInput = false;

					m_EditingReferenceIndex = pendingFromRefIndex == 0 ? 0 : newIndex;
					m_ForceCentralSelection = (pendingFromRefIndex == 0);
				}

				// スクロール調整
				{
					const float innerGap = 8.0f;
					const float panelW = btnW;
					const float opInnerGap = 50.0f;

					float firstRightX = baseX + btnW + innerGap;
					float colX = firstRightX;

					for (int i = 1; i < newIndex; ++i)
					{
						float opsX = colX + panelW + opInnerGap;
						float opsRight = opsX + btnW;
						colX = opsRight + innerGap;
					}

					float newColX = colX;

					ImVec2 winPos = ImGui::GetWindowPos();
					float scrollX = ImGui::GetScrollX();
					ImVec2 prevCursor = ImGui::GetCursorPos();

					float newColLocalX = newColX - winPos.x + scrollX;

					ImGui::SetCursorPos(ImVec2(newColLocalX, prevCursor.y));
					ImGui::Dummy(ImVec2(1.0f, 1.0f));

					ImGui::SetScrollHereX(0.5f);

					ImGui::SetCursorPos(prevCursor);
				}
			}
		}

		requestAddPending = false;
		pendingPressedIndex = -1;
		pendingPressedIsReal = false;
		pendingFromRefIndex = -1;
	}

	SafeSetCursorPos(prevCursorLocal.x, prevCursorLocal.y);
}

/**
* @brief 演算子ボタン描画
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::DrawOperatorPanel(int PackID, int CardID)
{

}

/**
* @brief 参照ラベル付け処理
* @param [in]パック番号
* @param [in]カード番号
*/
void My::CEdit::ReassignReferenceLabels(int PackID, int CardID)
{
	// 範囲チェック
	if (!IsValidPackIndex((size_t)PackID) || !IsValidCardIndex((size_t)PackID, (size_t)CardID))
		return;

	auto& refs = m_Pack[PackID].cards[CardID].references;

	// ラベル付けのみ行う（仕様変更なし）
	// 既存のラベルを上書きして必ず "参照先N" の形式にする
	for (size_t i = 0; i < refs.size(); ++i)
	{
		refs[i].label = std::string(u8"参照先") + std::to_string(i + 1);
	}
}

ordered_json My::CEdit::BuildCardsJson()
{
	ordered_json root;
	root[u8"カード"] = ordered_json::array();

	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{
		const auto& pack = m_Pack[packIdx];

		for (size_t cardIdx = 0; cardIdx < pack.cards.size(); ++cardIdx)
		{
			const auto& card = pack.cards[cardIdx];

			ordered_json j;
			j["Pack ID"] = (int)packIdx + 1;	// パック番号
			j["Card ID"] = (int)cardIdx + 1;	// カード番号
			j["Card Name"] = trim_copy(card.name);	// カード名
			j["Card Name Ruby"] = trim_copy(card.ruby);	// カード名の読み方
			j["image"] = card.imagePath;	// カードイラスト画像
			j["cost"] = card.cost;	// コスト
			j["rarity"] = static_cast<int>(card.raritytype);	// レアリティ
			j["type"] = static_cast<int>(card.maintype);	// カードタイプ(攻撃/守備/アシスト)
			j["target"] = card.target;	// 参照の有無

			if (card.maintype == ASSIST)
			{// アシストの時にのみ保存する内容
				j["isOneTime"] = card.isOneTime;	// 効果が単発かどうか

				if (!card.isOneTime)
				{// 効果が単発じゃない場合
					j["time"] = card.time;	// 効果の発動時間
				}
			}

			switch (card.maintype)
			{// カードのタイプによって保存する内容
			case ATTACK:	// 攻撃タイプの場合
				j["attacktype"] = static_cast<int>(card.attacktype);	// 攻撃対象(全体攻撃/特定の相手を選んで攻撃/ランダム攻撃/自分を含めて攻撃)
				if (!card.target)
				{// 参照しない場合にのみ攻撃対象とダメージ数を保存
					j["power"] = card.damage;	// 攻撃力
				}

				break;
			case DEFENSE:	// 守備タイプの場合
				j["defensetype"] = static_cast<int>(card.defensetype);	// カウンターの有無
				j["guard"] = card.guard;	// 守備値
				if (card.defensetype == COUNTER)
				{// カウンターするカードの場合
					j["counter"] = card.counter;	// 反撃値
				}
				break;
			case ASSIST:	// アシストタイプの場合
				j["assisttype"] = static_cast<int>(card.assisttype);	// アシストの種類設定
				if (card.assisttype == HEAL)
				{// 回復カードの場合
					j["healtype"] = static_cast<int>(card.healtype);	// 回復対象の設定(全体回復/特定の相手を選んで回復/ランダム回復/自分を含めてランダム回復/自分だけ)
					j["heal"] = card.heal;	// 回復量
				}
				break;
			default:
				break;
			}
			
			if (!card.references.empty() && card.target)
			{// 参照するときにのみ保存する内容
				// まず参照配列の情報を一時配列にコピーしておく（互換性補正用）
				const auto& refs = card.references;
				int n = (int)refs.size();

				// op/real/show/active を一旦抽出しておく
				std::vector<int> opForRef(n, -1);
				std::vector<float> realForRef(n, 0.0f);
				std::vector<bool> showInputForRef(n, false);
				std::vector<bool> activeDetailForRef(n, false);

				for (int i = 0; i < n; ++i)
				{
					opForRef[i] = refs[i].operetorbutton;
					realForRef[i] = refs[i].realValue;
					showInputForRef[i] = refs[i].showInput;
					activeDetailForRef[i] = refs[i].activeDetailExplicit;
				}

				for (int i = 0; i + 1 < n; ++i)
				{
					// 左側に何も無く、右側に演算子が入っている場合は左へ移す
					if (opForRef[i] == -1 && opForRef[i + 1] != -1)
					{
						opForRef[i] = opForRef[i + 1];
						opForRef[i + 1] = -1;

						// 実数値関連も移す（もし実数演算子なら realValue/showInput を移動）
						if (opForRef[i] >= 4 && opForRef[i] <= 7)
						{
							realForRef[i] = realForRef[i + 1];
							realForRef[i + 1] = 0.0f;

							showInputForRef[i] = showInputForRef[i + 1];
							showInputForRef[i + 1] = false;

							activeDetailForRef[i] = activeDetailForRef[i + 1];
							activeDetailForRef[i + 1] = false;
						}
					}
				}

				// ここで各参照を JSON 化する（opForRef / realForRef を使って左側格納仕様で出力）
				ordered_json refArray = ordered_json::array();
				for (int i = 0; i < n; ++i)
				{
					const Reference& r = refs[i];
					ordered_json rj;

					rj["label"] = r.label;	// ラベル(参照先1・2...)
					rj["targetselect"] = r.targetselect;	// 参照の対象(自分/自分以外/自分を含めた誰か)
					rj["reference"] = r.reference;	// 参照先	
					rj["othertargetselect"] = r.othertargetselect;	// 発動条件(特定の条件/ランダム)
					rj["targetobject"] = r.targetobject;	// 対象物(ゾーン/エナジー/HP/残り時間)
					rj["zone"] = r.zone;	// ゾーン(山札/墓地/待機/手札/フィールド)
					rj["startpos"] = r.startpos;	// どっちから見るか(上/下)
					rj["searchwidth"] = r.searchwidth;	// 参照幅()
					rj["selecttype"] = r.selecttype;
					rj["cardtype"] = r.cardtype;
					rj["addcostcondition"] = r.addcostcondition;
					rj["judgeoriginalvalue"] = r.judgeoriginalvalue;
					rj["Attackoriginalvalue"] = r.Attackoriginalvalue;
					rj["Defenseoriginalvalue"] = r.Defenseoriginalvalue;
					rj["num"] = r.num;

					rj["realValue"] = realForRef[i];

					rj["judgeKind"] = r.judgeKind;
					rj["referencenum"] = r.referencenum;

					rj["operetorbutton"] = opForRef[i];

					refArray.push_back(rj);
				}

				j["references"] = refArray;
			}

			// 効果追加の保存
			if (!card.effects.empty())
			{
				ordered_json effectArray = ordered_json::array();

				for (const auto& ef : card.effects)
				{
					ordered_json ej;

					ej["type"] = (int)ef.type;

					if (ef.type == EFFECT_ENERGY)
					{
						// エナジー効果
						ej["energy"]["num"] = ef.energy.num;
						ej["energy"]["member"] = ef.energy.member;
						ej["energy"]["ope"] = ef.energy.ope;
						ej["energy"]["selecttarget"] = ef.energy.selecttarget;
					}
					if (ef.type == EFFECT_DAMAGE)
					{
						// ダメージ効果
						ej["damage"]["num"] = ef.damage.num;
						ej["damage"]["member"] = ef.damage.member;
						ej["damage"]["selecttarget"] = ef.damage.selecttarget;
					}
					if (ef.type == EFFECT_DRAW)
					{
						// ドロー効果
						ej["draw"]["num"] = ef.draw.num;
						ej["draw"]["member"] = ef.draw.member;
						ej["draw"]["selecttarget"] = ef.draw.selecttarget;
					}

					effectArray.push_back(ej);
				}

				j["effects"] = effectArray;
			}

			root[u8"カード"].push_back(j);
		}
	}

	return root;
}

// 保存前に参照配列を正規化する
void My::CEdit::NormalizeReference()
{
	// 全パック・全カードを走査して参照ラベルを振り直す
	for (size_t packIdx = 0; packIdx < m_Pack.size(); ++packIdx)
	{
		for (size_t cardIdx = 0; cardIdx < m_Pack[packIdx].cards.size(); ++cardIdx)
		{
			ReassignReferenceLabels(static_cast<int>(packIdx), static_cast<int>(cardIdx));
		}
	}
}

My::Reference My::Reference::FromJson(const ordered_json& j)
{
	Reference r;
	r.kind = json_get_int_safe(j, "kind", 0);
	r.label = json_get_string_safe(j, "label", std::string(u8"参照先"));
	r.targetselect = json_get_int_safe(j, "targetselect", 0);
	r.reference = json_get_int_safe(j, "reference", 0);
	r.othertargetselect = json_get_int_safe(j, "othertargetselect", 0);
	r.targetobject = json_get_int_safe(j, "targetobject", 0);
	r.zone = json_get_int_safe(j, "zone", 0);
	r.startpos = json_get_int_safe(j, "startpos", 0);
	r.searchwidth = json_get_int_safe(j, "searchwidth", 0);
	r.selecttype = json_get_int_safe(j, "selecttype", 0);
	r.cardtype = json_get_int_safe(j, "cardtype", 0);
	r.addcostcondition = json_get_int_safe(j, "addcostcondition", 0);
	r.judgeoriginalvalue = json_get_int_safe(j, "judgeoriginalvalue", 0);
	r.Attackoriginalvalue = json_get_int_safe(j, "Attackoriginalvalue", 0);
	r.Defenseoriginalvalue = json_get_int_safe(j, "Defenseoriginalvalue", 0);
	r.num = json_get_int_safe(j, "num", 0);
	r.realValue = json_get_float_safe(j, "realValue", 0.0f);
	r.judgeKind = json_get_bool_safe(j, "judgeKind", false);
	r.referencenum = json_get_int_safe(j, "referencenum", 0);

	// 新仕様: operetorbutton を読み込む（ファイルに無ければ -1）
	r.operetorbutton = json_get_int_safe(j, "operetorbutton", -1);

	// 範囲チェック: -1 または 0..7 の範囲に収める
	if (r.operetorbutton < -1 || r.operetorbutton > 7)
	{
		r.operetorbutton = -1;
	}

	// activeDetailExplicit はファイルキー名の揺れに対応して読み込む
	if (j.contains("activeDetailExplicit") && j["activeDetailExplicit"].is_boolean())
	{
		r.activeDetailExplicit = j["activeDetailExplicit"].get<bool>();
	}
	else if (j.contains("activeOpExplicit") && j["activeOpExplicit"].is_boolean())
	{
		r.activeDetailExplicit = j["activeOpExplicit"].get<bool>();
	}
	else
	{
		r.activeDetailExplicit = (r.operetorbutton != -1);
	}

	// showInput はファイルにあればそれを優先、なければ operetorbutton から推定する
	if (j.contains("showInput") && j["showInput"].is_boolean())
	{
		r.showInput = j["showInput"].get<bool>();
	}
	else
	{
		r.showInput = (r.operetorbutton >= 4 && r.operetorbutton <= 7);
	}

	// 安全化: realValue が非有限なら 0 にする
	if (!std::isfinite(r.realValue))
	{
		r.realValue = 0.0f;
	}

	// referencenum の下限チェック
	if (r.referencenum < 0)
	{
		r.referencenum = 0;
	}

	return r;
}

