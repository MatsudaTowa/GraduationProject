//===========================================
// 
// エフェクトシーン[scene_effect.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _SENE_TITLE_H_
#define _SENE_TITLE_H_
#include "scene_base.h"		// シーンベース
#include "../object/3D/object_3D.h"	// ３Dオブジェクト


namespace My
{
	class Effect;	// エフェクト
	class CText;	// テキスト
	namespace Scene {
		class CSene_Effekseer : public CBase
		{
		public:
			CSene_Effekseer(CBase* scene);
			~CSene_Effekseer();

		private:
			// 基礎
			virtual CBase* Update()	override;	// 更新
			virtual void Draw() const override;	// 描画
			bool GetPose()override;	// ポーズ状態取得

			// オブジェクトポインタ
			CObject3D* firld;	// 地面
			Effect* m_pEffect;	// エフェクト
			CText* m_pTextParameter;		// パラメーター説明テキスト
			CText* m_pTextExplanation;		// 全体説明テキスト

			// パラメーターの編集項目
			enum class enumSelect {
				MIN = 0,
				SELECT_POS = MIN,	// 位置の編集
				SELECT_ROT,	// 向きの編集
				SELECT_SCL,	// 拡大率の編集
				SELECT_COL,	// 色
				MAX
			};
			enumSelect m_Select;	// 選択項目
			// 操作
			void ParamExplanationUpdate();	// 全体の説明更新
			void PosExplanationUpdate();	// 位置編集の説明更新
			void RotExplanationUpdate();	// 向き編集の説明更新
			void SclExplanationUpdate();	// 拡大編集の説明更新
			void ColExplanationUpdate();	// 色編集の説明更新
			// 操作説明
			void ParamExplanationDraw();	// 全体の説明描画


			// テキスト位置
			const int m_nDefTextSpase[4];	// 説明テキストの位置の初期位置
			int m_nTextSpase[4];			// テキスト表示範囲

			// コンフィグ
			bool SaveConfigList();	// コンフィグ書き込み処理
			bool LoadConfigList();	// コンフィグ読み込み処理

			// パラメーター保存
			struct Param
			{
				std::string FilePas;	// ファイルパス
				D3DXVECTOR3 pos;		// 位置情報
				D3DXVECTOR3 rot;		// 向き情報
				D3DXVECTOR3 scl;		// スケール情報
				D3DXCOLOR col;			// 色情報
			};
			static const Param s_Param;

			bool SaveEffektParamText(const std::string FilePas);	// エフェクトパラメーター保存
			Param LoadEffektParamText(const std::string FilePas);	// エフェクトパラメーター保存
			bool SaveEffektParamBinary(const std::string FilePas);	// エフェクトパラメーター保存
			Param LoadEffektParamBinary(const std::string FilePas);	// エフェクトパラメーター保存

			// 説明保存
			bool SaveExplanationText(const std::string FilePas);	// 説明設定
			bool LoadExplanationText(const std::string FilePas);	// 説明設定

			// コンフィグ
			const std::string m_sConfigListFilepas;	// コンフィグファイルパス
			struct ConfigList {
				ConfigList();
				std::string m_sEffektFilepas;	// 読み込むエフェクトのファイルパス(txt)
				std::string m_sExplanationFilepas;	// 説明ファイルパス設定説明
			};
			static const ConfigList s_Config;	// コンフィグの初期値
			ConfigList m_Config;	// コンフィグ

			// 説明
			struct Explanation {
				Explanation();
				std::string m_Explanation;			// 全体設定説明
				std::string m_sParamExplanation;	// パラメーター編集説明
				std::string m_sMoveExplanation;		// 移動量設定説明
				std::string m_sRotExplanation;		// 回転量設定説明
				std::string m_sSclExplanation;		// 拡大量設定説明
				std::string m_sColExplanation;		// 変色量設定説明
			};
			static const Explanation s_Explanation;	// 説明格納用
			Explanation m_Explanation;	// 説明格納用

			struct InitialParamater {
				InitialParamater();
				const float m_move;		// 一回の移動量
				const float m_rol;		// 一回の回転量
				const float m_scl;		// 一回の拡大量
				const int m_colSRGB;	// 一回の変色量（SRGB）
			};
			static const InitialParamater s_InitialParamater;	// 初期値
		};
	}
}
#endif // !_SENE_TITLE_H_
