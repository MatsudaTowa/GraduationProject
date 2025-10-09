//======================================
//
//	�G�l�~�[�̊Ǘ������鏈��[enemymanager.cpp]
//	Author : Yuuto Shimadu
//
//======================================

//�w�b�_�[�̃C���N���[�h
#include "enemy_manager.h"
#include "manager.h"
//============================
//�R���X�g���N�^
//============================
My::CEnemyManager::CEnemyManager() :
	m_apManager()
{
	m_apManager.clear();
}

//============================
//�f�X�g���N�^
//============================
My::CEnemyManager::~CEnemyManager()
{
}

//============================
//������
//============================
HRESULT My::CEnemyManager::Init()
{
	return S_OK;
}

//============================
//�I������
//============================
void My::CEnemyManager::Uninit()
{
	for (auto& itr : m_apManager)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_apManager.clear();
}

//============================
//�o�^����
//============================
void My::CEnemyManager::Regist(CEnemy* enemy)
{
	//�G�̏���o�^
	m_apManager.push_back(enemy);
}

//============================
//�폜����
//============================
void My::CEnemyManager::Erase(CEnemy* enemy)
{
	//�T�C�Y��0�Ȃ甲����
	if (m_apManager.size() == 0)
	{
		return;
	}
	//�G�̏����폜
	m_apManager.remove(enemy);
}