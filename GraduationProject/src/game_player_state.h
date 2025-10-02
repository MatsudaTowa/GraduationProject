//=============================================
//
//�Q�[���v���C���[�̃X�e�[�g�p�^�[��[game_player_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_STATE_H_
#define _GAME_PLAYER_STATE_H_

#include "character.h"

/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CGamePlayer;

	/** @brief �v���C���[�̃X�e�[�g�N���X */
	class CGamePlayerState
	{
	public:
		CGamePlayerState() {};
		virtual ~CGamePlayerState() {};
		/**
		* @brief �j���[�g��������(�e�ł͉������Ȃ�)
		* @param [in]�Q�[���v���C���[�|�C���^
		*/
		virtual void Neutral([[maybe_unused]] CGamePlayer* player) {};
	};

	/** @brief �v���C���[�̃j���[�g�����N���X */
	class CNeutralState :public CGamePlayerState
	{
	public:
		/**
		* @brief �j���[�g��������
		* @param [in]�Q�[���v���C���[�|�C���^
		*/
		void Neutral(CGamePlayer* player) override;
	};
}
#endif // !_GAME_PLAYER_STATE_H_
