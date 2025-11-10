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
CCard::CCard() : 
	m_Param()
{

}

//=====================================
//デストラクタ
//=====================================
CCard::~CCard()
{

}

//=====================================
//攻撃パラメータの取得
//=====================================
CCard::AttackParam CCard::GetAttackParam(Param param)
{
	//攻撃パラメータ
	CCard::AttackParam AttackParam;

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
CCard::DefenseParam CCard::GetDefenseParam(Param param)
{
	//攻撃パラメータ
	CCard::DefenseParam DefenseParam;

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
CCard::AssistParam CCard::GetAssistParam(Param param)
{
	//攻撃パラメータ
	CCard::AssistParam AssistParam;

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