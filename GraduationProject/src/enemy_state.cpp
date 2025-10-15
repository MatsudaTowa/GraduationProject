////=============================================
////
////�G�l�~�[�̃X�e�[�g�p�^�[��[enemy_state.cpp]
////Author Matsuda Towa
////
////=============================================
//#include "enemy_state.h"
//#include "active_manager.h"
//#include "enemy.h"
//
////=============================================
//// �R���X�g���N�^
////=============================================
//My::CLobbyState::CLobbyState()
//{
//}
//
////=============================================
//// �f�X�g���N�^
////=============================================
//My::CLobbyState::~CLobbyState()
//{
//}
//
////=============================================
//// ���r�[����
////=============================================
//void My::CLobbyState::Lobby(CActiveSceneCharacter* character)
//{
//	if (typeid(*character) != typeid(CEnemy))
//	{
//		return;
//	}
//	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
//	//���[�V�����ݒ�
//	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
//}
//
////=============================================
//// �R���X�g���N�^
////=============================================
//My::CDuelState::CDuelState()
//{
//}
//
////=============================================
//// �f�X�g���N�^
////=============================================
//My::CDuelState::~CDuelState()
//{
//}
//
////=============================================
//// �f���G������
////=============================================
//void My::CDuelState::Duel(CActiveSceneCharacter* character)
//{
//	if (typeid(*character) != typeid(CEnemy))
//	{
//		return;
//	}
//	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
//	//UI����������ĂȂ���ΐ��������
//	CreateDuelUI(enemy);
//	//���[�V�����ݒ�
//	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
//}
//
////=============================================
//// UI�ݒ�
////=============================================
//void My::CDuelState::CreateDuelUI(CEnemy* enemy)
//{
//	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), enemy->GetPos()); //�X�N���[�����W�ɕϊ�
//
//	if (enemy->GetLifeUI() == nullptr)
//	{
//		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
//		enemy->SetLifeUI(pLifeUI);
//	}
//
//	if (enemy->GetEnergyUI() == nullptr)
//	{
//		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
//		enemy->SetEnergyUI(pEnergyUI);
//	}
//}
