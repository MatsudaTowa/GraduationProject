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

/** @brief My 名前空間 */
namespace My
{
	/** @brief キャラクタークラス */
	class CCharacter : public CObjectX
	{
	public:
		static const int MAX_KEY = 20;					//!<キー最大数
		static const int CHARACTER_PRIORITY = 8;		//!<キャラクターのプライオリティ
		static const int MAX_MOTION = 100;				//!<モーションの最大数
		static const int MAX_PARTS = 64;				//!<最大パーツ数
		static constexpr float KILL_Y = -200.0f;		 //!<これを下回ったら殺す

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
		 * @brief 重力
		 */		
		void Gravity();

		/**
		 * @brief ジャンプ処理
		 */		
		void Jump();

		/**
		 * @brief 床の判定
		 */		
		void HitField();

		/**
		 * @brief ワールドマトリックスに変換
		 */		
		void ConversionMtxWorld();

		/**
		 * @brief 移動量設定
		 * @param [in]移動量
		 */		
		inline void SetMove(D3DXVECTOR3 move)
		{
			m_move = move;
		}

		/**
		 * @brief 過去の位置設定
		 * @param [in]過去の位置
		 */
		inline void SetOldPos(D3DXVECTOR3 oldpos)
		{
			m_oldpos = oldpos;
		}

		/**
		 * @brief 着地しているか設定
		 * @param [in]過去の位置
		 */
		inline void SetLanding(bool bLanding)
		{
			m_bLanding = bLanding;
		}

		/**
		 * @brief ライフ設定
		 * @param [in]現在の体力
		 */
		inline void SetLife(int nLife)
		{
			m_nLife = nLife;
		}

		/**
		 * @brief ジャンプ数設定
		 * @param [in]ジャンプ数
		 */		
		inline void SetJumpCnt(int nJumpCnt)
		{
			m_nJumpCnt = nJumpCnt;
		}

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
		  * @brief 移動量取得
		  * @return 移動量
		  */		
		inline D3DXVECTOR3& GetMove()
		{
			return m_move;
		}

		 /**
		  * @brief 過去に位置取得
		  * @return 過去の位置
		  */	
		inline D3DXVECTOR3& GetOldPos()
		{
			return m_oldpos;
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
		 * @brief 着地しているか取得
		 * @return 着地している
		 */		
		inline bool& GetLaunding()
		{
			return m_bLanding;
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
		 * @brief 体力取得
		 * @return 現在の体力
		 */				
		inline int& GetLife()
		{
			return m_nLife;
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
		 * @brief 速度取得
		 * @return 現在の速度
		 */			
		inline float& GetSpeed()
		{
			return m_Speed;
		}

		/**
		 * @brief ジャンプ数取得
		 * @return 現在のジャンプ数
		 */	
		inline int& GetJumpCnt()
		{
			return m_nJumpCnt;
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
		static constexpr float MOVE_FRICTION = 0.3f; //!<移動抵抗
		static constexpr float SHADOW_POS_Y = 0.5f;	 //!<影のY座標(地面から少し浮かす)
		static constexpr float GRAVITY_MOVE = 2.0f;  //!<重力値
		static constexpr float GRAVITY_MAX = 100.0f; //!<重力最大値

		D3DXVECTOR3 m_move;			//!<速度
		D3DXVECTOR3 m_oldpos;		//!<過去の位置
		bool m_bLanding;			//!<着地してるかどうか
		bool m_isLoopFinish;		//!<ループモーションが終わったか
		int m_nLife;				//!<体力
		int m_PartsCnt;				//!<パーツ数
		int m_nMotionFrameCnt;		//!<切り替えフレームカウント
		int m_nKeySetCnt;			//!<キーカウント
		int m_nJumpCnt;				//!<ジャンプカウント
		int m_Motion;				//!<モーション(各オブジェクトから列挙を受け取る)
		float m_Speed;				//!<スピード
		float m_Jump;				//!<ジャンプ
		D3DXCOLOR m_col;			//!<カラー
		D3DXVECTOR3 m_ShadowSize;	//!<影のサイズ

		CShadow* m_pShadow;			//!<影

		CModel_Parts* m_apModel[MAX_PARTS]; 		//!<パーツ

		/**
		 * @brief キー構造体
		 */	
		struct Key
		{
			D3DXVECTOR3 s_pos;		//!<位置
			D3DXVECTOR3 s_rot;		//!<方向
			D3DXVECTOR3 s_Trot;		//!<Tポーズ時の方向
		};

		/**
		 * @brief キー設定構造体
		 */	
		struct KeySet
		{
			int s_nFrame;			//!<フレーム数
			Key s_key[MAX_KEY];		//!<キー情報
		};

		/**
		 * @brief モーション設定構造体
		 */	
		struct MotionSet
		{
			int s_nLoop;					//!<ループするかどうか
			int s_nNumKey;					//!<キー数
			KeySet s_keySet[MAX_PARTS];		//!<キー設定情報
		};

		/**
		 * @brief モーション読み込み情報構造体
		 */
		struct MotionData
		{
			MotionSet s_motion_set[MAX_MOTION];		//!<モーション設定
			int s_parts;							//!<パーツ数
			int s_idx[MAX_PARTS];					//!<パーツ番号
			int s_parent[MAX_PARTS];				//!<親の番号
			float s_speed;							//!<速度
			float s_jump;							//!<ジャンプ力
			char s_path[MAX_PARTS][256];		//!<ファイルパス
			D3DXVECTOR3 s_parts_pos[MAX_PARTS];		//!<パーツの位置
			D3DXVECTOR3 s_parts_rot[MAX_PARTS];		//!<パーツの方向
		};

		MotionData m_motion_data;	//!<モーション読み込みデータ
	};
}

#endif