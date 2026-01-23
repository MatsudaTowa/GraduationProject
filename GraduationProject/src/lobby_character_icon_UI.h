#ifndef _CLOBBY_CHARACTER_ICON_UI_
#define _CLOBBY_CHARACTER_ICON_UI_

namespace My
{
	class CLobbyCharacterIconUI:public CObject2D
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CLobbyCharacterIconUI(int nPriority = 3);

		/**
		* @brief デストラクタ
		*/
		~CLobbyCharacterIconUI()override;

		/**
		* @brief 初期化
		*/
		HRESULT Init()override;

		/**
		*  @brief 終了
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
		*/
		static CLobbyCharacterIconUI* Create(D3DXVECTOR3 pos, D3DXCOLOR color);

		void SetIndex(int index)
		{
			m_Idx = index;
		}

		int GetIndex()
		{
			return m_Idx;
		}
	private:
		int m_Idx;
	};
}

#endif // !_CLOBBY_CHARACTER_ICON_UI_