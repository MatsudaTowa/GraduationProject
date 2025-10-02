//=============================================
//
//3DTemplate[field.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _FIELD_H_ //これが定義されてないとき

#define _FIELD_H_
#include "main.h"
#include "object3D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief 床クラス
	 * @copydoc CObject3D TODO:XファイルやFBXになる可能性あり
	 */
	class CField : public CObject3D
	{
	public:

		static const int FIELD_PRIORITY = 3; //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CField(int nPriority = FIELD_PRIORITY);

		/**
		 * @brief デストラクタ
		 */	
		~CField()override;

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
		 * @brief 床生成
		 * @param [in]生成位置
		 * @param [in]生成サイズ
		 * @param [in]確保するポインタ
		 * @return 床のポインタ
		 */
		static CField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CField* field);
	private:
	};
}

#endif