//=============================================
//
//数字管理「number.h」
// Author松田永久
//
//=============================================
#ifndef _NUMBER_H_ //これが定義されてないとき
#define _NUMBER_H_
#include "main.h"
#include "object2D.h"
#include "billboard.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My 数字(2D)クラス */
	class CNumber_2D :public CObject2D
	{
	public:
		static const int NUMBER_PRIORITY = 29;  //描画順

		enum TEX_TYPE
		{
			TEX_000 = 0,
			TEX_001,
			MAX
		};

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CNumber_2D(int nPriority = NUMBER_PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CNumber_2D()override;
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
		 * @brief 数字生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @param [in]テクスチャ番号
		 * @return 数字の2Dポインタ
		 */
		static CNumber_2D* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int idx);

		/**
		 * @brief 数字設定
		 * @param [in]数字のテクスチャmin座標
		 * @param [in]数字のテクスチャmax座標
		 * @param [in]色
		 */
		void SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col);

		/**
		 * @brief 数字の頂点生成
		 */
		void NumberVtx();

	private:
		D3DXVECTOR2 m_mintex; //テクスチャの最小座標
		D3DXVECTOR2 m_maxtex; //テクスチャの最大座標
	};

	/** @brief My 数字(3D)クラス */
	class CNumber_3D :public CBillboard
	{
	public:
		static const int NUMBER_PRIORITY = 20;  //描画順
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CNumber_3D(int nPriority = NUMBER_PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CNumber_3D()override;
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
		 * @brief 数字生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @return 数字の3Dポインタ
		 */		
		static CNumber_3D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

		/**
		 * @brief 数字の設定
		 * @param [in]数字のテクスチャmin座標
		 * @param [in]数字のテクスチャmax座標
		 * @param [in]色
		 */
		void SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col);

		/**
		 * @brief 数字の頂点生成
		 */
		void NumberVtx();

	private:
		static const std::string TEXTURE_NAME;	//テクスチャの名前

		D3DXVECTOR2 m_mintex; //テクスチャの最小座標
		D3DXVECTOR2 m_maxtex; //テクスチャの最大座標
	};
}

#endif // NUMBER