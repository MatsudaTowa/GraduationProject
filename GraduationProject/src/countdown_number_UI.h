//=============================================
//
//カウントダウンの数字の表示UI[countdown_number_UI.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COUNTDOWN_NUMBER_UI_H_ //これが定義されてないとき

#define _COUNTDOWN_NUMBER_UI_H_
#include "countdown_UI.h"

namespace My
{
	//=============================================
	//カウントダウンナンバーUIクラス
	//=============================================
	class CCountdownNumberUI : public CCountdownUI
	{
	public:

		//カウントダウンに使うナンバー
		enum NUMBER
		{
			ZERO = 0,
			ONE,
			TWO,
			THREE
		};

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCountdownNumberUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CCountdownNumberUI()override;
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
		 * @return ポインタ
		 */
		static CCountdownNumberUI* Create(D3DXVECTOR3 pos, NUMBER num);

		/**
		 * @brief 引数に応じてサイズを変更
		 * @param [in]寿命
		 */
		void SetLife(float life) override 
		{
			//変数
			float fCurrentSize = GetSize().x;

			//進行時間分サイズを加算
			fCurrentSize += life;

			//サイズの設定
			SetSize({ fCurrentSize, fCurrentSize });

			CCountdownUI::SetLife(life);
		}

	private:

		//定数
		static constexpr float START_DELETE_LIFE = 0.2f;	//消え始める寿命の値
		static constexpr float START_SIZE = 70.0f;			//開始時のサイズ
		static constexpr float ADD_SIZE = 50.0f;			//加算されるサイズ
		static const std::string TEX_NAME;
		
	};
}

#endif