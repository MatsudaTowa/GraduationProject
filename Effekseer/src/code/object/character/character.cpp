//===========================================
// 
// キャラクター[charactor.cpp]
// Auther:UedaKou
// 
//===========================================
#include "character.h"

namespace My
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CCharacter::CCharacter()
	{
		m_nLife = 0;
		m_nCntInvincible = 0;
		m_nAttcak = 0;
		m_nDefense = 0;
		m_fSpeed = 0;
		m_Xcollision = X(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_xmove = X(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		bJump = false;
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CCharacter::~CCharacter()
	{
	}
	/// <summary>
	///  初期化処理
	/// </summary>
	/// <returns>true : 初期化成功</returns>
	HRESULT CCharacter::Init()
	{
		// モーションオブジェクト初期化
		CObjectMotion::Init();
		return S_OK;
	}
	/// <summary>
	/// 終了処理
	/// </summary>
	void CCharacter::Uninit()
	{
		// モーションオブジェクト終了処理
		CObjectMotion::Uninit();
	}
	/// <summary>
	/// 更新処理
	/// </summary>
	void CCharacter::Update()
	{
		// 動き加算
		CObjectMotion::AddPos(m_xmove.pos);
		CObjectMotion::AddRot(m_xmove.rot);

		// 移動減衰
		MoveAttenuation();

		// モーションオブジェクト更新
		CObjectMotion::Update();
		// 無敵時間が有ったらカウントダウンする
		if (m_nCntInvincible > 0)
		{
			m_nCntInvincible--;
		}
	}
	/// <summary>
	/// 描画処理
	/// </summary>
	void CCharacter::Draw()
	{
		// モーションオブジェクト描画
		CObjectMotion::Draw();
	}
	/// <summary>
	/// 次モーション取得
	/// </summary>
	/// <returns>次モーションを返す（派生クラスで設定しなければ0番目を返す）</returns>
	int CCharacter::GetNextMotion()
	{
		return 0;
	}
	/// <summary>
	/// ヒット処理(ダメージのみ)
	/// </summary>
	/// <param name="nDamage">ダメージ</param>
	void CCharacter::Hit(int nDamage)
	{
		if (m_nCntInvincible <= 0)
		{
			m_nCntInvincible = 0;
			m_nLife -= nDamage;
		}
	}
	/// <summary>
	/// ヒット処理(無敵時間とダメージのみ)
	/// </summary>
	/// <param name="nCntInvincible">無敵時間</param>
	/// <param name="nDamage">ダメージ</param>
	void CCharacter::Hit(int nCntInvincible, int nDamage)
	{
		if (m_nCntInvincible <= 0)
		{
			Hit(nDamage);
			m_nCntInvincible = nCntInvincible;
		}
	}
	/// <summary>
	/// ヒット処理(ノックバック,無敵時間とダメージ)
	/// </summary>
	/// <param name="ShockRot">ノックバック</param>
	/// <param name="nCntInvincible">無敵時間</param>
	/// <param name="nDamage">ダメージ</param>
	void CCharacter::Hit(D3DXVECTOR3 ShockRot, int nCntInvincible, int nDamage)
	{
		if (m_nCntInvincible <= 0)
		{
			Hit(nCntInvincible, nDamage);
			m_xmove.pos += ShockRot;
		}
	}
	/// <summary>
	/// 移動減衰
	/// </summary>
	void CCharacter::MoveAttenuation()
	{
		// yUp系移動減衰
		m_xmove.pos.x += (0 - m_xmove.pos.x) * s_fBase_Resistance;
		m_xmove.pos.z += (0 - m_xmove.pos.z) * s_fBase_Resistance;

		// 重力加算
		m_xmove.rot.y += (0 - m_xmove.rot.y) * s_fBase_Gravity;

	}
}