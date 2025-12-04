//================================
//
// カードのcpp[card.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "card.h"

//=====================================
//コンストラクタ
//=====================================
My::CCard::CCard() : 
	m_Param(),							//カードのパラメータ
	m_CardType(CCard_Client::NONE),		//カードの種類
	m_PreCalculateStrategy(),			//計算前効果のストラテジー
	m_PostCalculateStrategy(),			//計算後効果のストラテジー
	m_TargetIdVector(),					//ターゲット番号
	m_nUserId(-1),						//使用者番号
	m_CurrentZone(DECK),				//現在のゾーン
	m_OldZone(DECK),					//過去のゾーン
	m_BaseParam()						//ベースのパラメータ
{
	m_TargetIdVector.clear();
}

//=====================================
//デストラクタ
//=====================================
My::CCard::~CCard()
{

}

//=====================================
//攻撃パラメータの取得
//=====================================
CCard_Client::AttackParam My::CCard::GetAttackParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::AttackParam AttackParam;

	//代入
	AttackParam.BaseParam.nPackID = param.nPackID;		//パックID
	AttackParam.BaseParam.nCardID = param.nCardID;		//カードID
	AttackParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	AttackParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	AttackParam.BaseParam.Name = param.Name;			//カード名
	AttackParam.BaseParam.nCost = param.nCost;			//コスト
	AttackParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	AttackParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	AttackParam.Attacktype = param.Attacktype;			//攻撃種類
	AttackParam.nDamage = param.nDamage;				//ダメージ

	return AttackParam;
}

//=====================================
//防御パラメータの取得
//=====================================
CCard_Client::DefenseParam My::CCard::GetDefenseParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::DefenseParam DefenseParam;

	//代入
	DefenseParam.BaseParam.nPackID = param.nPackID;		//パックID
	DefenseParam.BaseParam.nCardID = param.nCardID;		//カードID
	DefenseParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	DefenseParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	DefenseParam.BaseParam.Name = param.Name;			//カード名
	DefenseParam.BaseParam.nCost = param.nCost;			//コスト
	DefenseParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	DefenseParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	DefenseParam.Defensetype = param.Defensetype;		//防御種類
	DefenseParam.nCounter = param.nCounter;				//カウンターダメージ
	DefenseParam.nGuard = param.nGuard;					//防御

	return DefenseParam;
}

//=====================================
//アシストパラメータの取得
//=====================================
CCard_Client::AssistParam My::CCard::GetAssistParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::AssistParam AssistParam;

	//代入
	AssistParam.BaseParam.nPackID = param.nPackID;		//パックID
	AssistParam.BaseParam.nCardID = param.nCardID;		//カードID
	AssistParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	AssistParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	AssistParam.BaseParam.Name = param.Name;			//カード名
	AssistParam.BaseParam.nCost = param.nCost;			//コスト
	AssistParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	AssistParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	AssistParam.Bufftype = param.Bufftype;				//バフ種類
	AssistParam.Healtype = param.Healtype;				//回復種類
	AssistParam.nHeal = param.nHeal;					//回復

	return AssistParam;
}