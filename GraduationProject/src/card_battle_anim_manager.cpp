#include "card_battle_anim_manager.h"

My::CCardBattleAnimManager::CCardBattleAnimManager():
	m_apManager()
{
	m_apManager.clear();
}

My::CCardBattleAnimManager::~CCardBattleAnimManager()
{
}

HRESULT My::CCardBattleAnimManager::Init()
{
	return S_OK;
}

void My::CCardBattleAnimManager::Uninit()
{
	for (auto& itr : m_apManager)
	{
		if (itr == nullptr)
		{
			continue;
		}
		delete itr;
		itr = nullptr;
	}
	m_apManager.clear();
	delete this;
}

void My::CCardBattleAnimManager::Regist(CCardBattleAnim* manager)
{
	//“G‚Ìî•ñ‚ğ“o˜^
	m_apManager.push_back(manager);
}

void My::CCardBattleAnimManager::Remove(CCardBattleAnim* manager)
{
	//ƒTƒCƒY‚ª0‚È‚ç”²‚¯‚é
	if (m_apManager.size() == 0)
	{
		return;
	}
	//“G‚Ìî•ñ‚ğíœ
	m_apManager.remove(manager);
}

void My::CCardBattleAnimManager::Update()
{
	std::vector<CCardBattleAnim*> removeList;

	for (auto& itr : m_apManager)
	{
		if (!itr) continue;

		if (itr->DuelAnim())
		{
			removeList.push_back(itr);
		}
	}

	for (auto* itr : removeList)
	{
		itr->Uninit();
		Remove(itr);
		delete itr;
	}
}
