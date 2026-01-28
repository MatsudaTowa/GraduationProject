//=============================================
//
//何Pかの表示UI[damage_number_UI.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DAMAGE_NUMBER_UI_H_ //これが定義されてないとき

#define _DAMAGE_NUMBER_UI_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class DamageNumberState;

	//=============================================
	//ダメージ数UIクラス
	//=============================================
	class CDamageNumberUI : public CObject2D
	{
	public:
		static const int PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CDamageNumberUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CDamageNumberUI()override;
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init()override;
		/**
		 * @brief 終了
		 */
		void Uninit()override;
		/**
		 * @brief 更新
		 */
		void Update()override;
		/**
		 * @brief 描画
		 */
		void Draw()override;

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]数
		 * @return
		 */
		static CDamageNumberUI* Create(D3DXVECTOR3 pos, int num);

		/**
		* @brief ステートの設定
		* @param [in]ステートのポインタ
		*/
		void SetState(DamageNumberState* state)
		{
			// 古いステートを削除
			if (m_pState != nullptr)
			{
				delete m_pState;
			}
			// 新しいステートを設定
			m_pState = state;
		}

		/**
		* @brief ステートの取得
		* @return ステートのポインタ
		*/
		DamageNumberState* GetState()
		{
			return m_pState;
		}
	private:
		static constexpr float SIZE = 30.0f;
		static const std::string TEX_NAME;
		float m_fLife;
		float m_num;

		/**
		* UI表記のステート
		*/
		DamageNumberState* m_pState;
	};

	
}

namespace My
{
	//=============================================
	// ダメージUIのステート基底クラス
	//=============================================
	class DamageNumberState
	{
	public:
		virtual void Update(CDamageNumberUI* ui) = 0;
	};

	//=============================================
	// ダメージUIのステート：開始時
	//=============================================
	class Start : public DamageNumberState
	{
	public:
		void Update(CDamageNumberUI* ui) override;

	private:
		// 最大サイズ
		static constexpr float MAX_SIZE = 45.0f;
	};

	//=============================================
	// ダメージUIのステート：待機時
	//=============================================
	class Wait : public DamageNumberState
	{
	public:
		void Update(CDamageNumberUI* ui) override;

	private:
		// 待機時間
		float m_fWaitTime = 0.3f;
	};

	//=============================================
	// ダメージUIのステート：終了時
	//=============================================
	class End : public DamageNumberState
	{
	public:
		void Update(CDamageNumberUI* ui) override;
	};
}

#endif