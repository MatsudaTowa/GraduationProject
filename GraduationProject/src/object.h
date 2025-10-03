//=============================================
//
//オブジェクト[object.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT_H_ //これが定義されてないとき

#define _OBJECT_H_
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief オブジェクトクラス */
	class CObject
	{
	public:
		static constexpr int MAX_OBJECT = 8192; //オブジェクトの最大数

		static constexpr int PRI_MAX = 30; //プライオリティの最大値

		enum OBJECT_TYPE
		{
			OBJECT_TYPE_NONE = 0,
			OBJECT_TYPE_TITLE,
			OBJECT_MOUSE,
			OBJECT_MOUSE_DEBUG,
			OBJECT_TYPE_TUTORIAL,
			OBJECT_TYPE_MOVE_POINT,
			OBJECT_TYPE_ATTACK_POINT,
			OBJECT_TYPE_START_POINT,
			OBJECT_TYPE_NUMBER,
			OBJECT_TYPE_RESULT,
			OBJECT_TYPE_FADE,
			OBJECT_TYPE_PLAYER,
			OBJECT_TYPE_BOARD_HORDER,
			OBJECT_TYPE_MUZZLE_FLUSH,
			OBJECT_TYPE_FIELD,
			OBJECT_TYPE_BOARD,
			OBJECT_TYPE_EFFECT,
			OBJECT_TYPE_BG,
			OBJECT_TYPE_SHADOW,
			OBJECT_TYPE_MAX,
		};

		explicit CObject(int nPriority = PRI_MAX);

		virtual ~CObject();
		virtual HRESULT Init() = 0;
		virtual void Uninit() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		static void ReleaseAll(); //全オブジェクト解放

		static void UpdateAll(); //全オブジェクト更新

		static void DrawAll(); //全オブジェクト描画


		/**
		 * @brief 先頭のオブジェクト取得
		 * @param [in]描画優先度
		 * @return 先頭オブジェクト
		 */
		static CObject* GetTopObject(int priority);

		/**
		 * @brief 次のオブジェクト取得
		 * @return 次のオブジェクト
		 */
		CObject* GetNext() { return m_pNext; }

		/**
		 * @brief 検索
		 * @param [in]リスト
		 */
		template<class T>
		void Search(std::list<T*>& list)
		{
			//プライオリティの数だけ周回
			for (int i = 0; i < PRI_MAX; i++)
			{
				CObject* pObj = m_pTop[i];	//先頭取得

				//最大数が不明なのでwhileを使用
				while (pObj != nullptr)
				{
					CObject* pNext = pObj->m_pNext;	//次のポインタを取得

					//キャストが成功したらリストに挿入
					T* pListObj = dynamic_cast<T*>(pObj);
					if (pListObj != nullptr)
					{
						list.push_back(pListObj);
					}

					pObj = pNext;					//ポインタを進める
				}
			}
		}

		inline OBJECT_TYPE GetType()
		{
			return m_type;
		}

		inline bool GetIsDelete()
		{
			return m_isDelete;
		}

		inline void SetisDraw(bool isDraw)
		{
			m_isDraw = isDraw;
		}

		inline void SetisDelete(bool isDelete)
		{
			m_isDelete = isDelete;
		}

		//オブジェクトの処理を止めるかを設定
		inline void SetStop(bool isStop) 
		{ 
			m_isStop = isStop; 
		}

		//オブジェクトの処理を止めるかを取得
		inline bool GetStop() 
		{ 
			return m_isStop; 
		}

	protected:
		void Release(); //自分自身の解放

		inline bool& GetisDraw()
		{
			return m_isDraw;
		}

		inline void SetType(OBJECT_TYPE type)
		{
			m_type = type;
		}

	private:
		static CObject* m_pTop[PRI_MAX];	//先頭のオブジェクトポインタ
		static CObject* m_pCur[PRI_MAX];	//現在（最後尾）のポインタ
		CObject* m_pPrev;						//前のオブジェクトのポインタ
		CObject* m_pNext;						//次のオブジェクトのポインタ
		int m_nPriority; //描画優先度

		static int m_nNumAll; //総数

		bool m_isStop;	//処理を止める

		bool m_isDelete;	//消すかどうか

		bool m_isDraw;		//描画するか

		int m_nID; //オブジェクトのID
		OBJECT_TYPE m_type;

	};
}
#endif