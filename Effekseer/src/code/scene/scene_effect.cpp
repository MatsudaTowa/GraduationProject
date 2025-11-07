//===========================================
// 
// タイトルシーン[scene_title.cpp]
// Auther:UedaKou
// 
//===========================================
#include "scene_effect.h"	// タイトルシーン
#include "../system/manager.h"	// 全体マネージャー
#include "../object/effect/Effect.h"	// エフェクト
#include "../system/pch.h"	// プリコンパイル(プリコンパイル済みヘッダで強制的に読み込んでいるのに時々読み込みエラー)

#include "SimpleIni.h"	// iniファイル読み込みに使用
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>

namespace My {
	namespace Scene {
		/// <summary>
		/// ファイルパス格納用コンストラクタ
		/// </summary>
		CSene_Effekseer::ConfigList::ConfigList():
			m_sEffektFilepas("data/Effect.txt"),
			m_sExplanationFilepas("data/Explanation.bin")
		{
			//m_sEffektFilepas("data/Flame.efk"),

		}
		/// <summary>
		/// 説明格納用データベースのコンストラクタ
		/// </summary>
		CSene_Effekseer::Explanation::Explanation() :
			m_Explanation(
				"【操作方法】\n"
				"<保存(テキスト)>       : <F8>\n"
				"<保存(バイナリ)>       : <F9>\n"
				"<読み込み(テキスト)>   : <F10>\n"
				"<読み込み(バイナリ)>   : <F11>\n"
				"<説明保存>             : <F12>\n"
			),
			m_sParamExplanation(
				"+--------------------------------+\n"
				"| <W><S> パラメーターの変更      |\n"
				"+--------------------------------+\n"
			),
			m_sMoveExplanation(
				"| <R><F>:X軸移動  <V>X軸リセット |\n"
				"| <T><G>:Y軸移動  <B>Y軸リセット |\n"
				"| <Y><H>:Z軸移動  <N>Z軸リセット |\n"
				"+--------------------------------+\n"
			),
			m_sRotExplanation(
				"| <R><F>:X軸回転  <V>X軸リセット |\n"
				"| <T><G>:Y軸回転  <B>Y軸リセット |\n"
				"| <Y><H>:Z軸回転  <N>Z軸リセット |\n"
				"+--------------------------------+\n"
			),
			m_sSclExplanation(
				"| <R><F>:X軸拡大  <V>X軸リセット |\n"
				"| <T><G>:Y軸拡大  <B>Y軸リセット |\n"
				"| <Y><H>:Z軸拡大  <N>Z軸リセット |\n"
				"+--------------------------------+\n"
			),
			m_sColExplanation(
				"| <R><F>:R値増減  <V>R値リセット |\n"
				"| <T><G>:G値増減  <B>G値リセット |\n"
				"| <Y><H>:B値増減  <N>B値リセット |\n"
				"| <U><J>:A値増減  <M>A値リセット |\n"
				"+--------------------------------+\n")
		{}

		/// <summary>
		/// 移動量
		/// </summary>
		CSene_Effekseer::InitialParamater::InitialParamater():
			m_move (10.0f),
			m_rol (D3DX_PI * 0.01f),
			m_scl (0.1f),
			m_colSRGB (5)
		{}



		// 静的メンバ変数定数
		const CSene_Effekseer::ConfigList CSene_Effekseer::s_Config;
		const CSene_Effekseer::Explanation CSene_Effekseer::s_Explanation;	// 説明格納用
		const CSene_Effekseer::InitialParamater CSene_Effekseer::s_InitialParamater;	// 初期値

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="scene">ベースのポインタ</param>
		CSene_Effekseer::CSene_Effekseer(CBase* scene) :
			CBase(*scene),
			firld(nullptr),
			m_pEffect(nullptr),
			m_pTextParameter(nullptr),
			m_pTextExplanation(nullptr),
			m_nDefTextSpase{ 1000, 0,SCREEN_W, SCREEN_H },
			m_nTextSpase{ m_nDefTextSpase[0], m_nDefTextSpase[1], m_nDefTextSpase[2] , m_nDefTextSpase[3] },
			m_sConfigListFilepas("data/ConfigList.ini")
		{
			LoadConfigList();

			// 説明読み込み
			LoadExplanationText(m_Config.m_sExplanationFilepas);

			Effect::Paramater param;
			// エフェクト読み込み
			param = LoadEffektParamText(m_Config.m_sEffektFilepas);

			// エフェクトm_Config.m_sEffektFilepas.c_str()
			m_pEffect = Effect::create(param.m_sFilepas);
			m_pEffect->SetPos(param.m_pos);
			m_pEffect->SetRot(param.m_rot);
			m_pEffect->SetScl(param.m_scl);
			m_pEffect->SetCol(Effect::ColorSRGBToFloat(param.m_col));
			m_pEffect->SetLoop(true);



			// パラメーター用テキスト
			m_pTextParameter = CText::create();
			m_pTextParameter->SetSpace(m_nTextSpase[0], m_nTextSpase[1], m_nTextSpase[2], m_nTextSpase[3]);

			// 説明用テキスト
			m_pTextExplanation = CText::create();

			// 説明描画
			ParamExplanationDraw();

			// フィールド
			firld = CObject3D::create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
			firld->SetColor(D3DXCOLOR(0.0f, 0.5f, 0.2f, 1.0f));
		}
		/// <summary>
		/// デストラクタ
		/// </summary>
		CSene_Effekseer::~CSene_Effekseer()
		{
			SaveConfigList();
		}
		/// <summary>
		/// 更新
		/// </summary>
		/// <returns>正常ならthisを返す：nullptrを返したら終了</returns>
		CBase* CSene_Effekseer::Update()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得

	
			// 選択更新
			ParamExplanationUpdate();
	

			if (pKye->GetTrigger(DIK_F1))
			{
				SaveExplanationText(m_Config.m_sExplanationFilepas);
			}
			if (pKye->GetTrigger(DIK_F11))
			{
				SaveEffektParamText(m_Config.m_sEffektFilepas);
			}


			return this;
		}
		/// <summary>
		/// 描画
		/// デバッグテキストはここで表示させrことがある
		/// </summary>
		void CSene_Effekseer::Draw() const
		{
		}
		/// <summary>
		/// ポーズ状態取得
		/// </summary>
		/// <returns>ポーズ状態を返す</returns>
		bool CSene_Effekseer::GetPose()
		{
			return false;
		}
		/// <summary>
		/// コンフィグファイル保存
		/// </summary>
		/// <returns>false:保存失敗</returns>
		bool CSene_Effekseer::SaveConfigList()
		{
			CSimpleIniA ini;
			// 値を取得（第3引数はデフォルト値）
			m_Config.m_sEffektFilepas = ini.GetValue("FILEPAS", "EffektFilepas", s_Config.m_sEffektFilepas.c_str());
			m_Config.m_sExplanationFilepas = ini.GetValue("FILEPAS", "ExplanationFilepas", s_Config.m_sExplanationFilepas.c_str());

			// 値を設定
			ini.SetValue("FILEPAS", "EffektFilepas", m_Config.m_sEffektFilepas.c_str());
			ini.SetValue("FILEPAS", "ExplanationFilepas", m_Config.m_sExplanationFilepas.c_str());

			// ファイルに保存
			SI_Error rc = ini.SaveFile(m_sConfigListFilepas.c_str());
			if (rc < 0) {
				std::cerr << "設定の保存に失敗しました\n";
			}
			return true;
		}
		/// <summary>
		/// コンフィグファイル読み込み
		/// </summary>
		/// <returns>false:読み込み失敗</returns>
		bool CSene_Effekseer::LoadConfigList()
		{
			CSimpleIniA ini;
			ini.LoadFile(m_sConfigListFilepas.c_str());

			// 値を取得（セクション名,キー名,デフォルト値）
			m_Config.m_sEffektFilepas = ini.GetValue("FILEPAS", "EffektFilepas", s_Config.m_sEffektFilepas.c_str());
			m_Config.m_sExplanationFilepas = ini.GetValue("FILEPAS", "ExplanationFilepas", s_Config.m_sExplanationFilepas.c_str());

			return true;
		}

		/// <summary>
		/// エフェクトのパラメーター保存(テキストファイル)
		/// </summary>
		/// <param name="FilePas">保存するファイルパス</param>
		/// <returns>false:保存失敗</returns>
		bool CSene_Effekseer::SaveEffektParamText(const std::string FilePas)
		{
			std::ofstream file(FilePas.c_str());  // 読み込むファイルのパスを指定
			if (!file) {
				return false;
			}
			// エフェクト情報取得
			Effect::Paramater param;

			if (m_pEffect)
			{
				param.m_sFilepas = 	m_pEffect->GetFilepas();			// ファイルパス取得
				param.m_pos = 		m_pEffect->GetPos();				// 位置取得
				param.m_rot = 		m_pEffect->GetRot();				// 向き取得
				param.m_scl = 		m_pEffect->GetScl();				// スケール取得
				param.m_col = 		m_pEffect->GetColSRGB();			// 色取得
				param.m_nLoopInterval = m_pEffect->GetLoopInterval();	// ループ間隔
				param.m_bIsLoop = m_pEffect->IsLoop();					// ループするか
				param.m_nLife = m_pEffect->GetLife();					// 寿命
			}

			// 書き込み
			file << "Filepas = "<< param.m_sFilepas << std::endl;
			file << "POS = " << param.m_pos.x << " " << param.m_pos.y << " " << param.m_pos.z << std::endl;
			file << "ROT = " << param.m_rot.x << " " << param.m_rot.y << " " << param.m_rot.z << std::endl;
			file << "SCL = " << param.m_scl.x << " " << param.m_scl.y << " " << param.m_scl.z << std::endl;
			file << "COL = " << param.m_col.R << " " << param.m_col.G << " " << param.m_col.B << " " << param.m_col.A <<std::endl;
			file << "LOOP_INTEWRVAL = " << param.m_nLoopInterval << std::endl;
			file << "LOOP = " << param.m_bIsLoop << std::endl;
			file << "LIFE = " << param.m_nLife << std::endl;


			file.close(); // ファイルを閉じる

			return true;
		}
		/// <summary>
		/// エフェクトのパラメーター読み込み(テキストファイル)
		/// </summary>
		/// <param name="FilePas">ファイルパス</param>
		/// <returns>false:読み込み失敗</returns>
		Effect::Paramater CSene_Effekseer::LoadEffektParamText(const std::string FilePas)
		{
			Effect::Paramater param;

			std::ifstream inFile(FilePas);
			if (!inFile) {
				std::cerr << "ファイルを開けませんでした\n";
				return param;
			}

			std::string str0, str1, str2, str3;	// 文字列格納用
			std::string skip;		// スキップ用格納

			std::string line;
			while ((std::getline(inFile, line)))
			{
				std::istringstream iss(line);
				std::string key;
				if (!(iss >> key)) continue; // 空行スキップ

				if (key == "Filepas") {
					std::string eq;
					iss >> eq >> param.m_sFilepas; // '=' をスキップして読み込む
				}
				else if (key == "POS") {
					char eq;
					iss >> eq >> param.m_pos.x >> param.m_pos.y >> param.m_pos.z;
				}
				else if (key == "ROT") {
					char eq;
					iss >> eq >> param.m_rot.x >> param.m_rot.y >> param.m_rot.z;
				}
				else if (key == "SCL") {
					char eq;
					iss >> eq >> param.m_scl.x >> param.m_scl.y >> param.m_scl.z;
				}
				else if (key == "COL") {
					char eq;
					iss >> eq >> param.m_col.R >> param.m_col.G >> param.m_col.B >> param.m_col.A;
				}
				else if (key == "LOOP_INTEWRVAL") {
					char eq;
					iss >> eq >> param.m_nLoopInterval;
				}
				else if (key == "LOOP") {
					char eq;
					iss >> eq >> param.m_bIsLoop;
				}
				else if (key == "LIFE") {
					char eq;
					iss >> eq >> param.m_nLife;
				}
			}

			inFile.close();
			return param;
		}
		//bool CSene_Effekseer::SaveEffektParamBinary(const std::string FilePas)
		//{
		//	// エフェクト情報取得
		//	D3DXVECTOR3 pos = m_pEffect->GetPos();
		//	D3DXVECTOR3 rot = m_pEffect->GetRot();
		//	D3DXVECTOR3 scl = m_pEffect->GetScl();
		//	Effekseer::Color col = m_pEffect->GetColSRGB();

		//	std::ofstream File(FilePas, std::ios::binary);
		//	if (!File.is_open())
		//	{
		//		return false;
		//	}

		//	// 文字列を書き込むラムダ
		//	auto WriteString = [&](const std::string& s) {
		//		size_t len = s.size();
		//		File.write(reinterpret_cast<const char*>(&len), sizeof(len)); // 長さ
		//		File.write(s.data(), len);                                     // 本文
		//	};

		//	// 順番に書き込む
		//	WriteString(m_Explanation.m_Explanation);
		//	WriteString(m_Explanation.m_sParamExplanation);
		//	WriteString(m_Explanation.m_sMoveExplanation);
		//	WriteString(m_Explanation.m_sRotExplanation);
		//	WriteString(m_Explanation.m_sSclExplanation);
		//	WriteString(m_Explanation.m_sColExplanation);

		//	File.close();
		//	return true;
		//}
		//Effect::Paramater CSene_Effekseer::LoadEffektParamBinary(const std::string FilePas)
		//{
		//	Effect::Paramater param;
		//	//ファイルの読み込み
		//	std::ifstream File(FilePas, std::ios::binary);
		//	if (!File.is_open())
		//	{
		//		return param;
		//	}
		//	File.seekg(0, std::ios::end);
		//	std::streamsize fileSize = File.tellg();
		//	File.seekg(0, std::ios::beg);

		//	if (fileSize < sizeof(Explanation)) {
		//		// 不正なファイル
		//		return param;
		//	}
		//	// 読み込み用ラムダ
		//	auto ReadString = [&](std::string& s) {
		//		size_t len = 0;
		//		File.read(reinterpret_cast<char*>(&len), sizeof(len)); // 文字列の長さを読む
		//		s.resize(len);                                        // 文字列領域を確保
		//		File.read(&s[0], len);                                // 本文を読む
		//	};

		//	// 順番に読み込む
		//	ReadString(m_Explanation.m_Explanation);
		//	ReadString(m_Explanation.m_sParamExplanation);
		//	ReadString(m_Explanation.m_sMoveExplanation);
		//	ReadString(m_Explanation.m_sRotExplanation);
		//	ReadString(m_Explanation.m_sSclExplanation);
		//	ReadString(m_Explanation.m_sColExplanation);

		//	File.close();
		//	return param;
		//}
		/// <summary>
		/// 説明用テキスト保存処理
		/// </summary>
		/// <param name="">保存ファイルパス</param>
		bool CSene_Effekseer::SaveExplanationText(const std::string FilePas)
		{
			std::ofstream File(FilePas, std::ios::binary);
			if (!File.is_open())
			{
				return false;
			}

			// 文字列を書き込むラムダ
			auto WriteString = [&](const std::string& s) {
				size_t len = s.size();
				File.write(reinterpret_cast<const char*>(&len), sizeof(len)); // 長さ
				File.write(s.data(), len);                                     // 本文
			};

			// 順番に書き込む
			WriteString(s_Explanation.m_Explanation);
			WriteString(s_Explanation.m_sParamExplanation);
			WriteString(s_Explanation.m_sMoveExplanation);
			WriteString(s_Explanation.m_sRotExplanation);
			WriteString(s_Explanation.m_sSclExplanation);
			WriteString(s_Explanation.m_sColExplanation);

			File.close();
			return true;
		}
		/// <summary>
		/// 説明用テキスト読み込み処理
		/// </summary>
		/// <param name="">読み込みファイルパス</param>
		bool CSene_Effekseer::LoadExplanationText(const std::string FilePas)
		{
			//ファイルの読み込み
			std::ifstream File(FilePas, std::ios::binary);
			if (!File.is_open())
			{
				return false;
			}
			File.seekg(0, std::ios::end);
			std::streamsize fileSize = File.tellg();
			File.seekg(0, std::ios::beg);

			if (fileSize < sizeof(Explanation)) {
				// 不正なファイル
				return false;
			}
			// 読み込み用ラムダ
			auto ReadString = [&](std::string& s) {
				size_t len = 0;
				File.read(reinterpret_cast<char*>(&len), sizeof(len)); // 文字列の長さを読む
				s.resize(len);                                        // 文字列領域を確保
				File.read(&s[0], len);                                // 本文を読む
			};

			// 順番に読み込む
			ReadString(m_Explanation.m_Explanation);
			ReadString(m_Explanation.m_sParamExplanation);
			ReadString(m_Explanation.m_sMoveExplanation);
			ReadString(m_Explanation.m_sRotExplanation);
			ReadString(m_Explanation.m_sSclExplanation);
			ReadString(m_Explanation.m_sColExplanation);

			File.close();

			return true;
		}
		/// <summary>
		/// 種類選択の更新
		/// </summary>
		void CSene_Effekseer::ParamExplanationUpdate()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得
			enumSelect SelectOld = m_Select;	// 前の選択を記録
			if (pKye->GetTrigger(DIK_W))
			{
				// 減らす
				m_Select = static_cast<enumSelect>(static_cast<int>(m_Select) - 1);
				if (m_Select < enumSelect::MIN)
				{
					m_Select = static_cast<enumSelect>(static_cast<int>(enumSelect::MAX) - 1);
				}
			}
			else if (pKye->GetTrigger(DIK_S))
			{
				// 増やす
				m_Select = static_cast<enumSelect>(static_cast<int>(m_Select) + 1);
				if (m_Select >= enumSelect::MAX)
				{
					m_Select = enumSelect::MIN;
				}
			}
			// 選択が変更されていたら
			if (m_Select != SelectOld)
			{
				// 描画する
				ParamExplanationDraw();
			}
			// 選択別更新処理
			switch (m_Select)
			{
			case My::Scene::CSene_Effekseer::enumSelect::SELECT_POS:
				PosExplanationUpdate();
				break;
			case My::Scene::CSene_Effekseer::enumSelect::SELECT_ROT:
				RotExplanationUpdate();
				break;
			case My::Scene::CSene_Effekseer::enumSelect::SELECT_SCL:
				SclExplanationUpdate();
				break;
			case My::Scene::CSene_Effekseer::enumSelect::SELECT_COL:
				ColExplanationUpdate();
				break;
			default:
				break;
			}

			return;
		}
		/// <summary>
		/// 位置情報編集説明
		/// </summary>
		void CSene_Effekseer::PosExplanationUpdate()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得
			bool bInKye = false;	// 変更が加わっているかの判定に私用

			// 移動
			if (pKye->GetTrigger(DIK_R))
			{	// 右
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ My::Scene::CSene_Effekseer::s_InitialParamater.m_move, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_F))
			{	// 手間
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ -s_InitialParamater.m_move, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_T))
			{	// 右
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ 0.0f, s_InitialParamater.m_move, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_G))
			{	// 右
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ 0.0f, -s_InitialParamater.m_move, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_Y))
			{	// 奥
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ 0.0f, 0.0f,s_InitialParamater.m_move });
			}
			else if (pKye->GetTrigger(DIK_H))
			{	// 左
				bInKye = true;	// 変更が加わった
				m_pEffect->AddPos({ 0.0f, 0.0f, -s_InitialParamater.m_move });
			}
			else if (pKye->GetTrigger(DIK_V))
			{	// X0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetPos();
				m_pEffect->SetPos({ 0.0f, pos.y, pos.z });
			}
			else if (pKye->GetTrigger(DIK_B))
			{	// Y0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetPos();
				m_pEffect->SetPos({ pos.x, 0.0f, pos.z });
			}
			else if (pKye->GetTrigger(DIK_N))
			{	// Z0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetPos();
				m_pEffect->SetPos({ pos.x, pos.y, 0.0f });
			}

			// 変更が加わっていたら
			if (bInKye)
			{
				ParamExplanationDraw();	// テキスト描画
			}
		}
		/// <summary>
		/// 向き情報編集説明
		/// </summary>
		void CSene_Effekseer::RotExplanationUpdate()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得
			bool bInKye = false;	// 変更が加わっているかの判定に私用

			// 移動
			if (pKye->GetTrigger(DIK_R))
			{	// X
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ s_InitialParamater.m_rol, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_F))
			{	// -X
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ -s_InitialParamater.m_rol, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_T))
			{	// Y
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ 0.0f, s_InitialParamater.m_rol, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_G))
			{	// -Y
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ 0.0f, -s_InitialParamater.m_rol, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_Y))
			{	// Z
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ 0.0f, 0.0f, s_InitialParamater.m_rol });
			}
			else if (pKye->GetTrigger(DIK_H))
			{	// -Z
				bInKye = true;	// 変更が加わった
				m_pEffect->AddRot({ 0.0f, 0.0f, -s_InitialParamater.m_rol });
			}
			else if (pKye->GetTrigger(DIK_V))
			{	// X0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 rot = m_pEffect->GetRot();
				m_pEffect->SetRot({ 0.0f, rot.y, rot.z });
			}
			else if (pKye->GetTrigger(DIK_B))
			{	// Y0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 rot = m_pEffect->GetRot();
				m_pEffect->SetRot({ rot.x, 0.0f, rot.z });
			}
			else if (pKye->GetTrigger(DIK_N))
			{	// Z0
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 rot = m_pEffect->GetRot();
				m_pEffect->SetRot({ rot.x, rot.y, 0.0f });
			}

			// 変更が加わっていたら
			if (bInKye)
			{
				ParamExplanationDraw();	// テキスト描画
			}
		}
		/// <summary>
		/// スケール情報編集説明
		/// </summary>
		void CSene_Effekseer::SclExplanationUpdate()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得
			bool bInKye = false;	// 変更が加わっているかの判定に私用

			// 移動
			if (pKye->GetTrigger(DIK_R))
			{	// X
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ s_InitialParamater.m_scl, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_F))
			{	// -X
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ -s_InitialParamater.m_scl, 0.0f, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_T))
			{	// Y
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ 0.0f, s_InitialParamater.m_scl, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_G))
			{	// -Y
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ 0.0f, -s_InitialParamater.m_scl, 0.0f });
			}
			else if (pKye->GetTrigger(DIK_Y))
			{	// Z
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ 0.0f, 0.0f, s_InitialParamater.m_scl });
			}
			else if (pKye->GetTrigger(DIK_H))
			{	// -Z
				bInKye = true;	// 変更が加わった
				m_pEffect->AddScl({ 0.0f, 0.0f, -s_InitialParamater.m_scl });
			}
			else if (pKye->GetTrigger(DIK_V))
			{	// X1
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetScl();
				m_pEffect->SetScl({ 1.0f, pos.y, pos.z });
			}
			else if (pKye->GetTrigger(DIK_B))
			{	// Y1
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetScl();
				m_pEffect->SetScl({ pos.x, 1.0f, pos.z });
			}
			else if (pKye->GetTrigger(DIK_N))
			{	// Z1
				bInKye = true;	// 変更が加わった
				D3DXVECTOR3 pos = m_pEffect->GetScl();
				m_pEffect->SetScl({ pos.x, pos.y, 1.0f });
			}

			// 変更が加わっていたら
			if (bInKye)
			{
				ParamExplanationDraw();	// テキスト描画
			}
		}
		/// <summary>
		/// 色情報編集説明
		/// </summary>
		void CSene_Effekseer::ColExplanationUpdate()
		{
			// マネージャー
			CManager* pManager = CManager::GetInstance();	// マネージャー取得
			CInputKeyboard* pKye = pManager->GetInKey();	// キーボード取得
			bool bInKye = false;	// 変更が加わっているかの判定に私用

			// 移動
			if (pKye->GetTrigger(DIK_R))
			{	// R
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ s_InitialParamater.m_colSRGB, 0, 0, 0 });
			}
			else if (pKye->GetTrigger(DIK_F))
			{	// -R
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ -s_InitialParamater.m_colSRGB, 0, 0, 0 });
			}
			else if (pKye->GetTrigger(DIK_T))
			{	// G
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0,s_InitialParamater.m_colSRGB,  0, 0 });
			}
			else if (pKye->GetTrigger(DIK_G))
			{	// -G
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0, -s_InitialParamater.m_colSRGB, 0, 0 });
			}
			else if (pKye->GetTrigger(DIK_Y))
			{	// B
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0, 0, s_InitialParamater.m_colSRGB, 0 });
			}
			else if (pKye->GetTrigger(DIK_H))
			{	// -B
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0, 0, -s_InitialParamater.m_colSRGB, 0 });
			}
			else if (pKye->GetTrigger(DIK_U))
			{	// A
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0, 0, 0 ,s_InitialParamater.m_colSRGB });
			}
			else if (pKye->GetTrigger(DIK_J))
			{	// -A
				bInKye = true;	// 変更が加わった
				m_pEffect->AddColorSRGB({ 0, 0, 0 , -s_InitialParamater.m_colSRGB });
			}
			else if (pKye->GetTrigger(DIK_V))
			{	// X0
				bInKye = true;	// 変更が加わった
				Effekseer::Color col = m_pEffect->GetColSRGB();
				m_pEffect->SetColorSRGB({ 255, col.G, col.B , col.A });
			}
			else if (pKye->GetTrigger(DIK_B))
			{	// Y0
				bInKye = true;	// 変更が加わった
				Effekseer::Color col = m_pEffect->GetColSRGB();
				m_pEffect->SetColorSRGB({ col.R, 255, col.B , col.A });
			}
			else if (pKye->GetTrigger(DIK_N))
			{	// Z0
				bInKye = true;	// 変更が加わった
				Effekseer::Color col = m_pEffect->GetColSRGB();
				m_pEffect->SetColorSRGB({ col.R, col.G, 255 , col.A });
			}
			else if (pKye->GetTrigger(DIK_M))
			{	// Z0
				bInKye = true;	// 変更が加わった
				Effekseer::Color col = m_pEffect->GetColSRGB();
				m_pEffect->SetColorSRGB({ col.R, col.G, col.B , 255 });
			}

			// 変更が加わっていたら
			if (bInKye)
			{
				ParamExplanationDraw();	// テキスト描画
			}
		}
		/// <summary>
		/// パラメーター描画
		/// </summary>
		void CSene_Effekseer::ParamExplanationDraw()
		{

			// エフェクト情報取得
			D3DXVECTOR3 pos = m_pEffect->GetPos();
			D3DXVECTOR3 rot = m_pEffect->GetRot();
			D3DXVECTOR3 scl = m_pEffect->GetScl();
			Effekseer::Color col = m_pEffect->GetColSRGB();

			std::ostringstream aStrParam, aStrExplanation;	// テキスト格納用
			aStrParam << std::fixed << std::setprecision(1); // 小数点以下1桁
			// aStrParam
			{
				aStrParam << "Effect\n";	// ここで読み込みファイルのパスや名前を入れたい
				// 編集項目分繰り返す
				for (int nCnt = 0; nCnt < static_cast<int>(enumSelect::MAX); nCnt++)
				{
					if (static_cast<int>(m_Select) == nCnt)
					{
						aStrParam << ">>";
					}
					else
					{
						aStrParam << "  ";
					}

					switch (static_cast<enumSelect>(nCnt))
					{
					case My::Scene::CSene_Effekseer::enumSelect::SELECT_POS:
						aStrParam << "Pos : X " << pos.x << " , Y " << pos.y << " , Z" << pos.z << " \n";
						break;
					case My::Scene::CSene_Effekseer::enumSelect::SELECT_ROT:
						aStrParam << "Rot : X " << rot.x << " , Y " << rot.y << " , Z" << rot.z << " \n";
						break;
					case My::Scene::CSene_Effekseer::enumSelect::SELECT_SCL:
						aStrParam << "Scl : X " << scl.x << " , Y " << scl.y << " , Z" << scl.z << " \n";
						break;
					case My::Scene::CSene_Effekseer::enumSelect::SELECT_COL:
						aStrParam << "Col : R " << static_cast<int>(col.R) << " , G " << static_cast<int>(col.G) << " , B" << static_cast<int>(col.B) << " A" << static_cast<int>(col.A) << " \n";
						break;
					default:
						break;
					}
				}
				// 整形した文字列をデバッグテキストに表示
				m_pTextParameter->SetText(aStrParam.str());
			}
			// aStrExplanation
			{
				aStrExplanation << m_Explanation.m_Explanation;	// 全体説明追加
				aStrExplanation << m_Explanation.m_sParamExplanation;	// パラメーター選択説明追加

				switch (m_Select)
				{
				case My::Scene::CSene_Effekseer::enumSelect::SELECT_POS:
					aStrExplanation << m_Explanation.m_sMoveExplanation;
					break;
				case My::Scene::CSene_Effekseer::enumSelect::SELECT_ROT:
					aStrExplanation << m_Explanation.m_sRotExplanation;
					break;
				case My::Scene::CSene_Effekseer::enumSelect::SELECT_SCL:
					aStrExplanation << m_Explanation.m_sSclExplanation;
					break;
				case My::Scene::CSene_Effekseer::enumSelect::SELECT_COL:
					aStrExplanation << m_Explanation.m_sColExplanation;
					break;
				default:
					break;
				}
				// 整形した文字列をデバッグテキストに表示
				m_pTextExplanation->SetText(aStrExplanation.str());
			}
		}
		/// <summary>
		/// シーン生成テンプレート
		/// </summary>
		/// <returns></returns>
		template<>
		Scene::CBase* CBase::makeScene<CSene_Effekseer>() {
			return new CSene_Effekseer(this);
		}

	}
}