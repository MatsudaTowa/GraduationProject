//=============================================
//
//�Q�[���v���C���[�̃X�e�[�g�p�^�[��[game_player_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player_state.h"
#include "game_player.h"
#include "active_manager.h"
#include "field.h"

//=============================================
// �j���[�g��������
//=============================================
void My::CNeutralState::Neutral(CGamePlayer* player)
{
	//���[�V�����ݒ�
	player->SetMotion(CPlayer::MOTION_NEUTRAL);
}