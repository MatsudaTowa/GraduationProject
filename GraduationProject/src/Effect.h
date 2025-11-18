//============================================
//
// エフェクト[Effekseer.h]
// Author:Uedakou
// 
//============================================
#ifndef EFFEKSEER_H_
#define EFFEKSEER_H_

#include <EffekseerForDXLib.h>  // DxLibも含む
#include "object.h"			// オブジェクト

// 使い方例
// 	My::CEffect::create("data/EFFECT/Effect.txt");

namespace My
{
	// エフェクト管理クラス
	class CEffect : public CObject
	{
	public:
		// 変数
		struct Paramater
		{
			Paramater();
			std::string		 m_sFilepas;		// ファイルパス
			D3DXVECTOR3		 m_pos;				// 位置
			D3DXVECTOR3		 m_rot;				// 向き(現在未使用)
			D3DXVECTOR3		 m_scl;				// スケール
			Effekseer::Color m_col;				// 色 (255)
			int				 m_nLoopInterval;	// エフェクトを再生する周期
			bool			 m_bIsLoop;			// ループするか
			int				 m_nLife;			// 寿命(-1で無限)
		};

		CEffect();			// コンストラクタ
		~CEffect()override;			// デストラクタ
		HRESULT Init()override;		// 初期化
		void Uninit()override;		// 終了
		void Update()override;		// 更新
		void Draw()override;		// 描画

		bool Load(_In_ const std::string sFilepas);	// 読み込み
		Effekseer::Handle Play();				// 再生

		D3DXVECTOR3 GetPos			() { return paramater.m_pos; }			// 位置の取得
		D3DXVECTOR3 GetRot			() { return paramater.m_rot; }			// 向きの取得（ラジアン）
		D3DXVECTOR3 GetScl			() { return paramater.m_scl; }			// スケールの取得
		Effekseer::Color	GetColSRGB	() { return paramater.m_col; }		// 色の取得SRGB
		D3DXCOLOR	GetCol();		// 色の取得SRGB
		bool		IsLoop			() { return paramater.m_bIsLoop; }			// ループするかどうか
		int			GetLoopInterval	() { return paramater.m_nLoopInterval; }	// ループのインターバル取得
		int			GetLife() { return paramater.m_nLife; }						// ループのインターバル取得
		std::string GetFilepas() { return paramater.m_sFilepas; }				// ファイルパス取得

		void SetPos(_In_ const D3DXVECTOR3 pos);								// 位置の設定
		void SetRot(_In_ const D3DXVECTOR3 rot);								// 向きの設定（ラジアン）
		void SetScl(_In_ const D3DXVECTOR3 scl);								// スケールの設定
		void SetScl(_In_ const float scl);										// スケールの設定（一括）
		void SetCol(_In_ const D3DXCOLOR col);									// 色(float)設定
		void SetColorSRGB(_In_ const Effekseer::Color col);					// 色(SRGB)の設定
		void SetLoop(_In_ const bool bLoop) { paramater.m_bIsLoop = bLoop; }	// ループするかどうか
		void SetLoopInterval(_In_ const int nLoopInterval) { paramater.m_nLoopInterval = nLoopInterval; }	// ループのインターバル設定
		void SetLife(_In_ const int nLife) { paramater.m_nLife = nLife; }	// ループのインターバル設定

		void AddPos(_In_ const D3DXVECTOR3 pos);							// 位置の加算
		void AddRot(_In_ const D3DXVECTOR3 rot);							// 向きの加算（ラジアン）
		void AddScl(_In_ const D3DXVECTOR3 scl);							// スケールの加算
		void AddScl(_In_ const float scl);									// スケールの加算（一括）
		void AddColor(_In_ const D3DXCOLOR col);							// 色(float)設定
		void AddColorSRGB(_In_ const std::array<int, 4>& col);		// 色（SRGB）設定
		void AddLoopInterval(_In_ const int nLoopInterval) { paramater.m_nLoopInterval += nLoopInterval; }	// ループのインターバル加算
		void AddLife(_In_ const int nLife) { paramater.m_nLife += nLife; }	// ループのインターバル加算


		static CEffect* create(_In_ const std::string sFilepas);	// 生成
	public:
		static Effekseer::Color ColorFloatToSRGB(_In_ const D3DXCOLOR col);		// SRGBColorからfloatColorに変換
		static D3DXCOLOR ColorSRGBToFloat(_In_ const Effekseer::Color col);		// SRGBColorからfloatColorに変換
		std::u16string ConvertToUtf16(_In_ const std::string& utf8);
	private:
		// エフェクト実態
		Effekseer::EffectRef m_effectRef = nullptr;	// エフェクト
		Effekseer::Handle m_handle = 0;	// ハンドル

		Paramater paramater;	// パラメーター

		int m_nLoopCount;	// 周期敵に再生するためのカウント

	};

}
#endif // !EFFEKSEER_H_