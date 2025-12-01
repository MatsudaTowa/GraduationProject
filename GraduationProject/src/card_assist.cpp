#include "card_assist.h"
#include "active_scene_manager.h"

My::CCardAssist::CCardAssist(int nPriority):CCard(nPriority),
m_AssistType()
{
}

My::CCardAssist::~CCardAssist()
{
}

HRESULT My::CCardAssist::Init()
{
	SetCardType(CCard::CARDTYPE_::TYPE_ASSIST);
	CCard::Init();

	//トリガー時のストラテジーを代入TODO : 仮代入
	//SetCardStrategy(new CEnergyAdjust);

	std::vector<CCardStrategy_Base*> strategy_vector = GetPostCalculateVector();
	strategy_vector.push_back(new CEnergyAdjust);
	SetpostCalculateStrategyVector(strategy_vector);
	return S_OK;
}

void My::CCardAssist::Uninit()
{
	CCard::Uninit();
}

void My::CCardAssist::Update()
{
	CCard::Update();
}

void My::CCardAssist::Draw()
{
	CCard::Draw();
}

//===========================================================================================================
// カード情報読み込み
//===========================================================================================================
void My::CCardAssist::LoadCardData()
{
	//TODO:各ステータス読み込みしてから親クラスの読み込み
	CCard::LoadCardData();
}

//===========================================================================================================
//固有情報の読み込み
//===========================================================================================================
void My::CCardAssist::LoadUniqueInfo(CCard_Client::Param param)
{
	//アシストステータスの読み込み
	m_AssistType = (AssistType)param.Assisttype;	//アシストの種類
}

//===========================================================================================================
//キャストをしたかの確認
//===========================================================================================================
bool My::CCardAssist::IsCast(CDuelCharacter* duel)
{
	//キャラクターリスト
	for (auto& iter : CActiveSceneManager::GetInstance()->GetCharacterList())
	{
		//対象のエリアを持つプレイヤーを探す
		if (GetTarget() != iter->GetArea()) continue;

		return true;
	}

	return false;
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAssist::Cast(CDuelCharacter* /*duel*/)
{
	//ステイゾーンに追加
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardAssist::Trigger()
{
	//ステイ後に起動
}