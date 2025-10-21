//===========================================================================================================================================================
// 
// card_frame.cppのヘッダー [card_frame.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_H_
#define _CARD_FRAME_H_

#include "main.h"
#include "object3D.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief カードフレームクラス
	 * @copydoc CObject3D
	 */
	class CCardFrame :public CObject3D
	{
	public:
		/**
		* @brief フレームのタイプ列挙 |
		*  命名規則 -> FRAMETYPE_name
		* @param name = BASE [基盤]
		* @param name = ILLUST [イラストが入る部分]
		* @param name = TEXT [効果テキストが入る部分]
		* @param name = NAME [カード名が入る部分]
		* @param name = TYPE [カードタイプが入る部分]
		* @param name = COST [コストが入る部分]
		*/
		typedef enum
		{
			FRAMETYPE_BASE=0,
			FRAMETYPE_ILLUST,
			FRAMETYPE_TEXT,
			FRAMETYPE_NAME,
			FRAMETYPE_TYPE,
			FRAMETYPE_COST,
			FRAMETYPE_MAX
		}FRAMETYPE;

		const float CARD_WIDTH = 10.0f;
		const float CARD_HEIGHT = 15.0f;

		/** 
		* @brief カードフレームの情報構造体
		* @param pass : 画像ファイルのパス
		* @param offset : オフセット位置
		* @param size : サイズ
		*/
		typedef struct
		{
			std::string pass;
			D3DXVECTOR3 offset;
			D3DXVECTOR3 size;
		}CardFrameInfo;

		/* @brief コンストラクタ */
		CCardFrame(int priority = 6);

		/* @brief デストラクタ */
		~CCardFrame()override;

		/* @brief 初期化 */
		HRESULT Init();

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;

		/* @brief 描画 */
		void Draw()override;

		static CCardFrame* Create(FRAMETYPE type, CObject3D* pObj);

		/* @brief 親設定 */
		void SetParent(CObject3D*pObj);

		/* @brief 親設定 */
		inline void SetType(CCardFrame::FRAMETYPE type) { m_type = type; }
		inline FRAMETYPE GetType() { return m_type; }

	protected:
		CardFrameInfo m_CardFrameInfo;

	private:
		/** @brief カードフレームまでの相対パス */
		static std::string FramePass;

		/** @brief カードフレームの情報構造体の配列*/
		static CardFrameInfo m_FrameInfo[FRAMETYPE::FRAMETYPE_MAX];

		/** @brief 親のポインタ */
		CObject3D* m_pParent;

		/** @brief 何のフレームかを判別するタイプ */
		FRAMETYPE m_type;
	};

}



#endif // !CARD_FRAME_H_
