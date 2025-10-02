//=============================================
//
//カウントを実行するクラス[count.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COUNT_H_ //これが定義されてないとき

#define _COUNT_H_
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief カウントクラス */
	class CCount
	{
	public:

		/**
		 * @brief コンストラクタ
		 */		
		CCount() :m_Cnt(0), m_Frame(0) {};

		/**
		 * @brief デストラクタ
		 */
		~CCount() {};

		/**
		 * @brief 計測
		 * @return カウント
		 */
		int CountMeasure()
		{
			++m_Cnt;
			return m_Cnt;
		}

		/**
		 * @brief カウントアップ
		 * @return 終了しているかどうか
		 */
		bool CountUp()
		{
			if (m_Cnt >= m_Frame)
			{//フレーム数に到達していれば
				return true;
			}

			//カウントアップ
			++m_Cnt;
			return false;
		}

		/**
		 * @brief カウントダウン
		 * @return 終了しているかどうか
		 */
		bool CountDown()
		{
			if (m_Cnt <= 0)
			{//フレーム数に到達していれば
				return true;
			}
			//カウントダウン
			--m_Cnt;
			return false;
		}

		/**
		 * @brief カウント代入
		 * @param [in]代入するカウント数
		 */
		inline void SetCnt(int Cnt) { m_Cnt = Cnt; }

		/**
		 * @brief フレーム数代入
		 * @param [in]代入するフレーム数
		 */
		inline void SetFrame(int Frame) { m_Frame = Frame; }

		/**
		 * @brief 現在のカウント取得
		 * @return 現在のカウント
		 */
		inline int GetCnt() { return m_Cnt; }

		/**
		 * @brief 現在のフレーム取得
		 * @return 現在のフレーム
		 */
		inline int GetFrame() { return m_Frame; }
	private:
		int m_Frame;	//!<フレーム数
		int m_Cnt;		//!<カウント数
	};
}
#endif