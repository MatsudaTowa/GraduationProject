//===========================================================================================================================================================
// 
// アシストカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================

//ヘッダーのインクルード
#include "card_assist.h"
#include "card_strategy.h"
#include "raknet_server.h"
#include "duel_player_manager.h"
#include "duel_manager.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardAssist::CCardAssist():
m_AssistType()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCardAssist::~CCardAssist()
{
}

//===========================================================================================================
// 初期化
//===========================================================================================================
HRESULT My::CCardAssist::Init()
{
	CCard::SetCardType(CCard_Client::CardType::ASSIST);
	CCard::Init();

	//トリガー時のストラテジーを代入TODO : 仮代入
	AddPostCalculateStrategy(new CHeal);
	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCardAssist::Uninit()
{
	CCard::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCardAssist::Update()
{
	CCard::Update();
}

//===========================================================================================================
//固有情報の読み込み
//===========================================================================================================
void My::CCardAssist::LoadUniqueInfo(CCard_Client::Param param)
{
	
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAssist::Cast(CDuel_Player* /*duel*/)
{
	//ステイゾーンに追加
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardAssist::Trigger()
{
	//ステイ後に起動
	//クライアントにトリガー情報を送信
	CRakNet_Server::GetInstance()->SendTriggerCard(this);


	for (auto& calculate_itr : GetPostCalculateVector())
	{
		if (calculate_itr == nullptr) { continue; }

		for (auto& target_itr : GetTargetIdVector())
		{
			calculate_itr->Strategy(CDuel_Player_Manager::GetInstance()->GetDuelPlayer(target_itr), this);
		}
	}
	if (isOneTime)
	{
		//状態とゾーンの変更
		ChangeState(CCardState::CARD_CEMETERY, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId()));
		CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId())->GetZoneManager()->MoveZone(this, CastToZone(GetCurrentZone(), CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId())), CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId())->GetZoneManager()->GetCemetery(), true);
		SetCurrentZone(CCard::CEMETERY);
	}
}

//===========================================================================================================
//キャストカードの読み込み処理
//===========================================================================================================
bool My::CCardAssist::LoadCastInfo(RakNet::BitStream* bsin, CastDestination destination)
{
	int nTargetNum = 0;
	bsin->Read(nTargetNum);

	//周回
	for (int i = 0; i < nTargetNum; i++)
	{
		int nId = 0;
		bsin->Read(nId);
		AddTargetIdVector(nId);
	}

	//キャスト状態に変更
	SetStartCastTime(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime());
	ChangeState(My::CCardState::CARD_CAST, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId()));
	return true;
}