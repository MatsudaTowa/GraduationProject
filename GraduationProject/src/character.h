//=============================================
//
//キャラクター[character.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CHARACTER_H_ //これが定義されてないとき

#define _CHARACTER_H_
#include "main.h"
#include "objectX.h"
#include "model_parts.h"
#include "shadow.h"
#include "count.h"
#include "useful.h"
#include "life_UI.h"
#include "energy_UI.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief キャラクタークラス */
	class CCharacter : public CObjectX
	{
	public:
		static constexpr int MAX_KEY = 20;					//!<キー最大数
		static constexpr int CHARACTER_PRIORITY = 8;		//!<キャラクターのプライオリティ
		static constexpr int MAX_MOTION = 100;				//!<モーションの最大数
		static constexpr int MAX_PARTS = 64;				//!<最大パーツ数

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCharacter(int nPriority = CHARACTER_PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CCharacter()override;

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
		 * @brief モーションの描画
		 */	
		void MotionDraw();

		/**
		 * @brief パーツのロード
		 * @param [in]モデルのファイルネーム
		 */	
		void Load_Parts(const char* FileName);

		/**
		 * @brief モーション
		 */			
		void Motion();

		/**
		 * @brief 引数で指定したモーションに切り替える
		 * @param [in]モーションの列挙指定ナンバー
		 */			
		void SetMotion(int Motion);

		/**
		 * @brief ワールドマトリックスに変換
		 */		
		void ConversionMtxWorld();

		 /**
		  * @brief モーションのフィニッシュ設定
		  * @param [in]モーションのループ終了したか
		  */
		inline void SetFinish(bool isFinish)
		{
			m_isLoopFinish = isFinish;
		}

		 /**
		  * @brief 影のサイズ設定設定
		  * @param [in]影のサイズ
		  */
		inline void SetShadowSize(D3DXVECTOR3 shadow_size)
		{
			m_ShadowSize = shadow_size;
		}

		/**
		 * @brief 影のサイズ取得
		 * @return 影のサイズ
		 */			
		inline D3DXVECTOR3& GetShadowSize()
		{
			return m_ShadowSize;
		}

		/**
		 * @brief モーションのループが終わっているかどうか
		 * @return 終わっているかどうか
		 */		
		inline bool& GetFinish()
		{
			return m_isLoopFinish;
		}

		/**
		 * @brief パーツ数取得
		 * @return パーツ数
		 */	
		inline int& GetNumParts()
		{
			return m_PartsCnt;
		}

		/**
		 * @brief 現在のキー取得
		 * @return キー数
		 */
		inline int& GetKeyCnt()
		{
			return m_nKeySetCnt;
		}

		/**
		 * @brief 影取得
		 * @return 現在のジャンプ数
		 */
		inline CShadow* GetShadow()
		{
			return m_pShadow;
		}

		/**
		 * @brief パーツ取得
		 * @param [in]パーツ番号
		 * @return モデルパーツのポインタ
		 */
		inline CModel_Parts* GetModelParts(int idx)
		{
			return m_apModel[idx];
		}
	private:
		static constexpr float SHADOW_POS_Y = 0.5f;	 //!<影のY座標(地面から少し浮かす)

		bool m_isLoopFinish;		//!<ループモーションが終わったか
		int m_PartsCnt;				//!<パーツ数
		CCount* m_pMotionFrameCnt;	//!モーションフレームカウント
		int m_nKeySetCnt;			//!<キーカウント
		int m_Motion;				//!<モーション(各オブジェクトから列挙を受け取る)
		D3DXCOLOR m_col;			//!<カラー
		D3DXVECTOR3 m_ShadowSize;	//!<影のサイズ

		CShadow* m_pShadow;			//!<影

		CModel_Parts* m_apModel[MAX_PARTS]; 		//!<パーツ

		/**
		 * @brief キー構造体
		 */	
		struct Key
		{
			D3DXVECTOR3 pos;		//!<位置
			D3DXVECTOR3 rot;		//!<方向
			D3DXVECTOR3 Trot;		//!<Tポーズ時の方向
		};

		/**
		 * @brief キー設定構造体
		 */	
		struct KeySet
		{
			int nFrame;			//!<フレーム数
			Key key[MAX_KEY];		//!<キー情報
		};

		/**
		 * @brief モーション設定構造体
		 */	
		struct MotionSet
		{
			int nLoop;					//!<ループするかどうか
			int nNumKey;					//!<キー数
			KeySet keySet[MAX_PARTS];		//!<キー設定情報
		};

		/**
		 * @brief モーション読み込み情報構造体
		 */
		struct MotionData
		{
			MotionSet motion_set[MAX_MOTION];		//!<モーション設定
			int parts;							//!<パーツ数
			int idx[MAX_PARTS];					//!<パーツ番号
			int parent[MAX_PARTS];				//!<親の番号
			float speed;							//!<速度
			float jump;							//!<ジャンプ力
			char path[MAX_PARTS][256];		//!<ファイルパス
			D3DXVECTOR3 parts_pos[MAX_PARTS];		//!<パーツの位置
			D3DXVECTOR3 parts_rot[MAX_PARTS];		//!<パーツの方向
		};

		MotionData m_motion_data;	//!<モーション読み込みデータ
	};
}

#endif