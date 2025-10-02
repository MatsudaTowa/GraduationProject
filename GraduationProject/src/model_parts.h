//=============================================
// 
//親子関係のパーツ[model.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MODEL_PARTS_H_ //これが定義されてないとき

#define _MODEL_PARTS_H_
#include "main.h"
#include "objectX.h"

/** @brief My 名前空間 */
namespace My
{

	/** @brief モデルパーツクラス */
	class CModel_Parts
	{
	public:
		static const int MAX_MODEL = 256; //!<モデルの数

		/** @brief モデル情報構造体 */
		struct MODEL_INFO
		{
			LPD3DXMESH s_pMesh;			//!<メッシュ情報
			LPD3DXBUFFER s_pBuffMat;	//!<マテリアル情報
			DWORD s_dwNumMat;			//!<マテリアル数;
			char* s_ModelName;			//!<モデルネーム保存用
		};

		/**
		 * @brief コンストラクタ
		 */
		CModel_Parts();

		/**
		 * @brief デストラクタ
		 */
		virtual ~CModel_Parts();

		/**
		 * @brief モデル破棄
		 */
		virtual void Unload();

		/**
		 * @brief Xファイル設定
		 * @param [in]マテリアルバッファ
		 * @param [in]マテリアル数
		 * @param [in]メッシュ情報
		 */
		virtual void BindXFile(LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPD3DXMESH pMesh);

		/**
		 * @brief 描画
		 */
		virtual void Draw();

		/**
		 * @brief 描画
		 * @param [in]カラー
		 */
		virtual void Draw(D3DXCOLOR col);

		/**
		 * @brief ワールドマトリックス計算
		 */
		virtual void ConversionMtxWorld();

		/**
		 * @brief パーツ生成
		 * @param [in]位置
		 * @param [in]方向
		 * @param [in]モデルのファイルパス
		 * @return モデルのポインタ
		 */
		static CModel_Parts* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char* pModel_Path);

		/**
		 * @brief ワールドマトリックス設定
		 * @param [in]ワールドマトリックス
		 */
		void SetMtxWorld(D3DXMATRIX mtxWorld)
		{
			m_mtxWorld = mtxWorld;
		}

		/**
		 * @brief 位置設定
		 * @param [in]位置
		 */
		void SetPos(D3DXVECTOR3 pos)
		{
			m_pos = pos;
		};

		/**
		 * @brief 過去の位置設定
		 * @param [in]過去の位置
		 */
		void SetOldPos(D3DXVECTOR3 oldpos)
		{
			m_oldpos = oldpos;
		};

		/**
		 * @brief 方向設定
		 * @param [in]方向
		 */
		void SetRot(D3DXVECTOR3 rot)
		{
			m_rot = rot;
		};

		/**
		 * @brief 番号設定
		 * @param [in]番号
		 */
		void SetIdx(int idx)
		{
			m_nIdx = idx;
		}

		/**
		 * @brief 親の設定
		 * @param [in]親の番号
		 */
		void SetIdxParent(int parent)
		{
			m_nIdxModelParent = parent;
		}

		/**
		 * @brief T字の位置
		 * @param [in]位置座標
		 */
		void SetTPos(D3DXVECTOR3 Tpos)
		{
			m_Tpos = Tpos;
		}

		/**
		 * @brief T字の方向
		 * @param [in]方向
		 */
		void SetTRot(D3DXVECTOR3 Trot)
		{
			m_Trot = Trot;
		}

		/**
		 * @brief 位置取得
		 * @return 位置
		 */
		D3DXVECTOR3 GetPos()
		{
			return m_pos;
		};

		/**
		 * @brief 過去の位置取得
		 * @return 過去の位置
		 */
		D3DXVECTOR3 GetOldPos()
		{
			return m_oldpos;
		};

		/**
		 * @brief 方向取得
		 * @return 方向
		 */
		D3DXVECTOR3 GetRot()
		{
			return m_rot;
		};

		/**
		 * @brief Tポーズ位置取得
		 * @return Tポーズ位置
		 */
		D3DXVECTOR3 GetTPos()
		{
			return m_Tpos;
		};

		/**
		 * @brief Tポーズ方向取得
		 * @return Tポーズ方向
		 */
		D3DXVECTOR3 GetTRot()
		{
			return m_Trot;
		};

		/**
		 * @brief 番号
		 * @return モデル番号
		 */
		int GetIdx()
		{
			return m_nIdx;
		}

		/**
		 * @brief 親の取得
		 * @return 親の番号
		 */
		int GetIdxParent()
		{
			return m_nIdxModelParent;
		}

		/**
		 * @brief 最小値取得
		 * @return 最小値
		 */
		D3DXVECTOR3 GetMin()
		{
			return m_minpos;
		}

		/**
		 * @brief 最大値取得
		 * @return 最大値
		 */
		D3DXVECTOR3 GetMax()
		{
			return m_maxpos;
		}

		/**
		 * @brief 親の設定
		 * @param [in]親のモデルパーツ
		 */
		void SetParent(CModel_Parts* pParent);

		/**
		 * @brief ワールドマトリックス取得
		 * @return ワールドマトリックス
		 */		
		D3DXMATRIX& GetMtxWorld();

		/**
		 * @brief モデル情報取得
		 * @param [in]モデルの番号
		 * @return モデル情報
		 */
		MODEL_INFO GetModelInfo(int nIdx);

	private:
		LPDIRECT3DTEXTURE9 m_pTexture[256]; //!<テクスチャへのポインタ
		D3DXMATRIX m_mtxWorld;				//!<ワールドマトリックス
		MODEL_INFO m_ModelInfo[MAX_MODEL];	//!<モデル情報

		D3DXVECTOR3 m_pos;					//!<位置
		D3DXVECTOR3 m_rot;					//!<方向
		D3DXVECTOR3 m_Tpos;					//!<Tポーズの位置
		D3DXVECTOR3 m_Trot;					//!<Tポーズの方向
		int m_nIdx;							//!<何番目のパーツか
		int m_nIdxModelParent;				//!<親のインデックス

		D3DXVECTOR3 m_minpos;				//!<pos最小値
		D3DXVECTOR3 m_maxpos;				//!<pos最大値
		D3DXVECTOR3 m_oldpos;				//!<過去の位置
		int m_nNumAll;						//!<モデル総数

		CModel_Parts* m_pParent;			//!<親パーツへのポインタ
	};
}

#endif