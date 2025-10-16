//============================================
//
// エフェクト[Effekseer.h]
// Author:Uedakou
// 
//============================================
#ifndef EFFEKSEER_H_
#define EFFEKSEER_H_

#include <EffekseerForDXLib.h>  // DxLibも含む
#include "../object.h"

namespace My
{
	// エフェクト管理クラス
	class Effect : public CObject
	{
	public:
		Effect();			// コンストラクタ
		~Effect();			// デストラクタ
		HRESULT Init();		// 初期化
		void Uninit();		// 終了
		void Update();		// 更新
		void Draw();		// 描画

		//void SetPos(::D3DXVECTOR3 pos);

		static Effect* create(const std::string sFilepas);
	private:
		Effekseer::EffectRef g_effectRef = nullptr;
		Effekseer::Handle g_handle;

		// 変数
		const int	EffectPlayInterval = 300;					// エフェクトを再生する周期

		// 今回の動作でにみ必要な変数
		int		playCount;			// 周期敵に再生するためのカウント
	};

}
#endif // !EFFEKSEER_H_