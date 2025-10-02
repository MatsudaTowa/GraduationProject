//=============================================
//
//�e�N�X�`���Ǘ�[texture.cpp]
//Author Matsuda Towa
//
//=============================================
#include "texture.h"
#include "manager.h"

/**
 * @brief �R���X�g���N�^
 */
My::CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;
	}

	m_nNumAll = 0;
}

/**
 * @brief �f�X�g���N�^
 */
My::CTexture::~CTexture()
{
}

/**
 * @brief �e�N�X�`���j��
 */
void My::CTexture::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{

			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
			m_texName[nCnt] = nullptr;

		}
	}
	m_nNumAll = 0;

}

/**
 * @brief �e�N�X�`���o�^
 * @param [in]�e�N�X�`���̃t�@�C���p�X
 */
int My::CTexture::Regist(const std::string* pTex)
{
	int nIdx = 0;
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (m_apTexture[nCnt] == nullptr)
		{
			LPDIRECT3DDEVICE9 pDevice = GET_RENDERER->GetDevice();

			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pTex->c_str(),
				&m_apTexture[nCnt]);
			
			//�����̃t�@�C���p�X��ۑ�
			m_texName[nCnt] = (std::string*)pTex;
			nIdx = nCnt;	//�ԍ��̕ۑ�
			m_nNumAll++;	//�����̃J�E���g�A�b�v
			break;
		}
		else if (m_texName[nCnt] ==pTex)
		{//�����̃e�N�X�`�������݂���Ȃ�

			//�ԍ���������break
			nIdx = nCnt;
			break;
		}
	}
 	return nIdx;
}

/**
 * @brief �e�N�X�`���A�h���X�擾
 * @param [in]�ԍ�
 * @return �e�N�X�`���A�h���X
 */
LPDIRECT3DTEXTURE9 My::CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];
}
