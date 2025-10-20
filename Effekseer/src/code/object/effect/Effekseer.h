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

		bool Load(const std::string sFilepas, D3DXVECTOR3 pos);
		void SetPos(D3DXVECTOR3 pos);	// 位置の設定
		void SetRot(D3DXVECTOR3 rot);	// 向きの設定(ラジアン)
		void SetScl(D3DXVECTOR3 scl);	// スケールの設定
		void SetCol(D3DXCOLOR col);
		void SetLoop(bool bLoop) { m_bIsLoop = bLoop; }

		void AddPos(D3DXVECTOR3 pos);	// 位置の加算
		void AddRot(D3DXVECTOR3 rot);	// 向きの加算(ラジアン)
		void AddScl(D3DXVECTOR3 scl);	// スケールの加算
		void AddCol(D3DXCOLOR col);


		static Effect* create(const std::string sFilepas);
	private:
		// エフェクト実態
		Effekseer::EffectRef g_effectRef = nullptr;	// エフェクト
		Effekseer::Handle g_handle = 0;	// ハンドル

		// 変数
		std::string m_sFilepas;	// ファイルパス
		D3DXVECTOR3 m_pos;	// 位置
		D3DXVECTOR3 m_rot;	// 向き(現在未使用)
		D3DXVECTOR3 m_scl;	// スケール
		D3DXCOLOR m_col;
		int m_nLoopInterval = 300;					// エフェクトを再生する周期
		int m_nLoopCount;			// 周期敵に再生するためのカウント
		bool m_bIsLoop;
	};

}
#endif // !EFFEKSEER_H_