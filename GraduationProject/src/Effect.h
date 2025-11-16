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

		bool Load(const std::string sFilepas, D3DXVECTOR3 pos);
		Effekseer::Handle Play();

		D3DXVECTOR3 GetPos			() { return paramater.m_pos; }			// 位置の取得
		D3DXVECTOR3 GetRot			() { return paramater.m_rot; }			// 向きの取得（ラジアン）
		D3DXVECTOR3 GetScl			() { return paramater.m_scl; }			// スケールの取得
		Effekseer::Color	GetColSRGB	() { return paramater.m_col; }		// 色の取得SRGB
		D3DXCOLOR	GetCol();		// 色の取得SRGB
		bool		IsLoop			() { return paramater.m_bIsLoop; }			// ループするかどうか
		int			GetLoopInterval	() { return paramater.m_nLoopInterval; }	// ループのインターバル取得
		int			GetLife() { return paramater.m_nLife; }						// ループのインターバル取得
		std::string GetFilepas() { return paramater.m_sFilepas; }				// ファイルパス取得

		void SetPos(D3DXVECTOR3 pos);								// 位置の設定
		void SetRot(D3DXVECTOR3 rot);								// 向きの設定（ラジアン）
		void SetScl(D3DXVECTOR3 scl);								// スケールの設定
		void SetScl(float scl);										// スケールの設定（一括）
		void SetCol(D3DXCOLOR col);									// 色(float)設定
		void SetColorSRGB(Effekseer::Color col);					// 色(SRGB)の設定
		void SetLoop(bool bLoop) { paramater.m_bIsLoop = bLoop; }	// ループするかどうか
		void SetLoopInterval(int nLoopInterval) { paramater.m_nLoopInterval = nLoopInterval; }	// ループのインターバル設定
		void SetLife(int nLife) { paramater.m_nLife = nLife; }	// ループのインターバル設定

		void AddPos(D3DXVECTOR3 pos);							// 位置の加算
		void AddRot(D3DXVECTOR3 rot);							// 向きの加算（ラジアン）
		void AddScl(D3DXVECTOR3 scl);							// スケールの加算
		void AddScl(float scl);									// スケールの加算（一括）
		void AddColor(D3DXCOLOR col);							// 色(float)設定
		void AddColorSRGB(const std::array<int, 4>& col);		// 色（SRGB）設定
		void AddLoopInterval(int nLoopInterval) { paramater.m_nLoopInterval += nLoopInterval; }	// ループのインターバル加算
		void AddLife(int nLife) { paramater.m_nLife += nLife; }	// ループのインターバル加算


		static CEffect* create(const std::string sFilepas);	// 生成
	public:
		static Effekseer::Color ColorFloatToSRGB(const D3DXCOLOR col);		// SRGBColorからfloatColorに変換
		static D3DXCOLOR ColorSRGBToFloat(const Effekseer::Color col);		// SRGBColorからfloatColorに変換
		std::u16string ConvertToUtf16(const std::string& utf8);
	private:
		// エフェクト実態
		Effekseer::EffectRef m_effectRef = nullptr;	// エフェクト
		Effekseer::Handle m_handle = 0;	// ハンドル

		Paramater paramater;	// パラメーター

		int m_nLoopCount;	// 周期敵に再生するためのカウント

	};

}
#endif // !EFFEKSEER_H_