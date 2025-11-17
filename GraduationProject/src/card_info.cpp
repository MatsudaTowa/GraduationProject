////===========================================================================================================================================================
//// 
//// カード情報 [card_info.cpp]
//// Author : Kasai Keisuke
//// 
////===========================================================================================================================================================
//
//// include
//#include "card_info.h"
//
///**
//* @brief コンストラクタ
//*/
//My::CCardInfo::CCardInfo():
//	m_Param()
//{
//
//}
//
///**
//* @brief デストラクタ
//*/
//My::CCardInfo::~CCardInfo()
//{
//
//}
//
///**
//* @brief カード情報の生成
//* @param [in]バイト列
//* @return 成功したかどうか
//*/
//bool My::CCardInfo::LoadBytes(const std::vector<uint8_t>& bytes)
//{
//	if (bytes.empty())
//	{
//		std::cerr << "bytes empty" << std::endl;
//		return false;
//	}
//
//	try
//	{
//		nlohmann::ordered_json tmp = nlohmann::ordered_json::from_cbor(bytes);
//
//		if (!tmp.is_object())
//		{
//			std::cerr << "not object" << std::endl;
//		}
//
//		if (!tmp.contains("Pack ID") || !tmp.contains("Card ID"))
//		{
//			std::cerr << "miss required" << std::endl;
//		}
//
//		// 内部データ読み込み
//		bool bLoad = LoadJson(tmp);
//
//		if (!bLoad)
//		{
//			std::cerr << "Load failed" << std::endl;
//
//			return false;
//		}
//
//		return true;
//	}
//	catch (const nlohmann::ordered_json::parse_error& pe)
//	{// パースエラー
//		std::cerr << "LoadBytes parse_error:" << pe.what() << '\n';
//		return false;
//	}
//	catch (const std::exception& e)
//	{// ライブラリ例外
//		std::cerr << "LoadBytes exception:" << e.what() << '\n';
//		return false;
//	}
//}
//
///**
//* @brief オブジェクトを読み込む
//* @param [in]Jsonオブジェクト
//* @return 成功したかどうか
//*/
//bool My::CCardInfo::LoadJson(const nlohmann::ordered_json& j)
//{
//	//try
//	//{
//	//	if (!j.is_object())
//	//	{
//	//		std::cerr << "LoadJson: input is not an object";
//	//		return false;
//	//	}
//	//	if (!j.contains("Pack ID") || !j.contains("Card ID") || !j.contains("Card Name"))
//	//	{
//	//		std::cerr << "LoadJson: required key miss";
//	//		return false;
//	//	}
//
//	//	// 情報読み込み
//	//	BaseParam baseParam{};
//	//	AttackParam attackParam{};		// 攻撃タイプ
//	//	DefenseParam defenseParam{};	// 守備タイプ
//	//	AssistParam assistParam{};		// アシストタイプ
//
//	//	// 共通情報
//	//	baseParam.nCardID = j["Card ID"].get<int>();
//	//	baseParam.nPackID = j["Pack ID"].get<int>();
//
//	//	baseParam.Name = j.value("Card Name", std::string{});
//	//	baseParam.Ruby = j.value("Card Name Ruby", std::string{});
//	//	baseParam.nCost = j.value("cost", 0);
//	//	baseParam.Maintype = static_cast<CardType>(j.value("type", static_cast<int>(NONE)));
//	//	baseParam.Raritytype = static_cast<RARITY>(j.value("rarity", static_cast<int>(NONE_RARITY)));
//	//	baseParam.ImagePath = j.value("image", std::string{});
//
//	//	// カードの個別情報
//	//	switch (baseParam.Maintype)
//	//	{
//	//	case ATTACK:
//	//	{
//	//		attackParam.nDamage = j.value("power", 0);
//	//		attackParam.Attacktype = j.value("attacktype", NONE_ATTACK);
//	//		if (attackParam.nDamage < 0)
//	//		{
//	//			std::cerr << "ダメージ数が登録されていません" << std::endl;
//
//	//		}
//	//		if (attackParam.Attacktype == NONE_ATTACK)
//	//		{
//	//			std::cerr << "攻撃対象が設定されていません" << std::endl;
//	//		}
//	//		break;
//	//	}
//	//	case DEFENSE:
//	//	{
//	//		defenseParam.nGuard = j.value("guard", 0);
//	//		defenseParam.nCounter = j.value("counter", 0);
//	//		defenseParam.Defensetype = j.value("defensetype", NONE_DEFENSE);
//	//		break;
//	//	}
//	//	case ASSIST:
//	//	{
//	//		assistParam.nHeal = j.value("heal", 0);
//	//		assistParam.Bufftype = j.value("bufftype", NONE_BUFF);
//	//		assistParam.Healtype = j.value("healtype", NONE_HEAL);
//	//		break;
//	//	}
//	//	default:
//	//		break;
//	//	}
//
//	//	m_Param = std::move(baseParam);
//	//	return true;
//	//}
//	//catch (const nlohmann::json::exception& je) 
//	//{
//	//	std::cerr << "LoadJson json exception: " << je.what() << '\n';
//	//	return false;
//	//}
//	//catch (const std::exception& e) 
//	//{
//	//	std::cerr << "LoadJson exception: " << e.what() << '\n';
//	//	return false;
//	//}
//
//	try
//	{
//		if (!j.is_object())
//		{
//			std::cerr << "オブジェクトが空です" << std::endl;
//			return false;
//		}
//
//		auto parseOne = [&](const nlohmann::ordered_json& item, CardRecord& outRec)->bool
//		{
//			if (!item.is_object())
//			{
//				return false;
//			}
//
//			if (!item.contains("Pack ID") || !item.contains("Card ID") || !item.contains("Card Name"))
//			{// タグが見つからなかった場合
//				return false;
//			}
//
//			// BaseParam組み立て
//			try 
//			{
//				outRec.baseParam.nPackID = item["Pack ID"].get<int>();	// パック番号
//				outRec.baseParam.nCardID = item["Card ID"].get<int>();
//			}
//			catch (const nlohmann::json::exception& e) {
//				// JSON ライブラリ由来のエラー（キー存在はしていたが型が違う等）
//				std::cerr << "JSON parse error for Pack ID/Card ID: " << e.what() << '\n';
//				return false;
//			}
//			catch (const std::exception& e) {
//				// その他の標準例外
//				std::cerr << "unexpected error reading Pack ID/Card ID: " << e.what() << '\n';
//				return false;
//			}
//
//			// 共通パラメーター
//			outRec.baseParam.Name = item.value("Card Name", std::string{});
//			outRec.baseParam.Ruby = item.value("Card Name Ruby", std::string{});
//			outRec.baseParam.nCost = item.value("cost", 0);
//			outRec.baseParam.Maintype = static_cast<CardType>(item.value("type", static_cast<int>(NONE)));
//			outRec.baseParam.Raritytype = static_cast<RARITY>(item.value("rarity", static_cast<int>(NONE_RARITY)));
//			outRec.baseParam.ImagePath = item.value("image", std::string{});
//			outRec.type = outRec.baseParam.Maintype;
//
//			// カード特有パラメーター
//			switch (outRec.type)
//			{
//			case ATTACK:
//				break;
//			case DEFENSE:
//				break;
//			case ASSIST:
//				break;
//			}
//		}
//	}
//	catch (const nlohmann::json::exception& je)
//	{
//		std::cerr << "LoadJson json exception: " << je.what() << '\n';
//		return false;
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << "LoadJson exception: " << e.what() << '\n';
//		return false;
//	}
//}
//
///**
//* @brief パラメーターの取得
//* @return パラメーター
//*/
//My::CCardInfo::BaseParam My::CCardInfo::GetParam()
//{
//	return m_Param;
//}
//
//const My::CCardInfo::CardRecord& My::CCardInfo::GetRecord()
//{
//	return m_Record;
//}
//
///**
//* @brief パラメーターの更新
//* @param [in]パラメーター
//*/
//void My::CCardInfo::UpdateParam(BaseParam& param)
//{
//	m_Param = param;
//}
