//=============================================
//
//当たり判定管理[colision.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COLISION_H_ //これが定義されてないとき

#define _COLISION_H_

/** @brief My 名前空間 */
namespace My
{
	/** @brief 当たり判定クラス */
	class CColision
	{
	public:
		/** @brief どこに当たったかの列挙 */
		enum COLISION
		{
			COLISON_NONE = 0,	//!<何にも当たってない
			COLISON_X,			//!<X方向に当たってる
			COLISON_Z,			//!<Z方向に当たってる
			COLISON_UNDER_Y,	//!<Y方向(下)に当たってる
			COLISON_TOP_Y,		//!<Y方向(上)当たってる
			COLISION_CIRCLE,	//!<円状に当たっている
		};

		/** @brief 円の判定情報構造体 */
		struct CIRCLE
		{
			COLISION colision;		//!<どう当たったか
			float CenterDistance;	//!<中心点との距離
		};

		/**
		 * @brief コンストラクタ
		 */		
		CColision();

		/**
		 * @brief デストラクタ
		 */			
		~CColision();

		/**
		 * @brief X軸のポリゴン同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */
		COLISION CheckColision_X_2D(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Bpos, D3DXVECTOR2 Bsize);

		/**
		 * @brief X軸のポリゴン同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */	
		COLISION CheckColision_X(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief X軸のモデル同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */
		COLISION CheckColision_X(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Y軸のポリゴン同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */		
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Y軸のモデルとポリゴン当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Y軸のモデル同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */
		COLISION CheckColision_Y(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Z軸のポリゴン同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 Asize, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Z軸のモデルとポリゴン当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 Bsize);

		/**
		 * @brief Z軸のモデル同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */		
		COLISION CheckColision_Z(D3DXVECTOR3 Aoldpos, D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxPos,
			D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief 球のモデル同士当たり判定チェック関数
		 * @param [in]当たる側の過去の位置
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側の最小値
		 * @param [in]当たる側の最大値
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */
		COLISION CheckColisionSphere(D3DXVECTOR3 Apos, D3DXVECTOR3 AMinpos, D3DXVECTOR3 AMaxpos, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief 円状の当たり判定
		 * @param [in]当たる側の位置
		 * @param [in]半径
		 * @param [in]当たられる側の位置
		 * @return 円のヒット情報
		 */
		CIRCLE CheckColisionCircle(D3DXVECTOR3 Apos, float Radius, D3DXVECTOR3 Bpos);

		/**
		 * @brief ポリゴンとモデル当たり判定チェック関数_球
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */		
		COLISION CheckPolygonModelColisionSphere(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);


		/**
		 * @brief X軸のポリゴンとモデル当たり判定チェック関数
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */
		COLISION CheckPolygonModelColision_X(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief Z軸のポリゴンとモデル当たり判定チェック関数
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */		
		COLISION CheckPolygonModelColision_Z(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);

		/**
		 * @brief ポリゴンとモデル当たり判定(埋まってるか)チェック関数
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側の最小値
		 * @param [in]当たられる側の最大値
		 * @return 当たっているか
		 */
		COLISION CheckPolygonFillColision(D3DXVECTOR3 Apos, D3DXVECTOR3 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR3 BMinpos, D3DXVECTOR3 BMaxpos);
		
		/**
		 * @brief 2Dポリゴン当たり判定チェック関数
		 * @param [in]当たる側の現在の位置
		 * @param [in]当たる側のサイズ
		 * @param [in]当たられる側の位置
		 * @param [in]当たられる側のサイズ
		 * @return 当たっているか
		 */
		bool Check2DPolygonColision(D3DXVECTOR3 Apos, D3DXVECTOR2 ASize, D3DXVECTOR3 Bpos, D3DXVECTOR2 BSize)
		{
			if (Apos.x - ASize.x < Bpos.x + BSize.x
				&& Apos.x + ASize.x > Bpos.x - BSize.x
				&& Apos.y - ASize.y < Bpos.y + BSize.y
				&& Apos.y + ASize.y > Bpos.y - BSize.y)
			{
				return true;
			}

			return false;
		}
	};
}

#endif