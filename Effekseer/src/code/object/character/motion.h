//===========================================
// 
// モーション[motion.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "../X/object_X.h"	// Xファイルオブジェクト

namespace My
{

#define MAX_MOTION (64)			// 最大モーション
#define MAX_KYE (32)			// 最大キーフレーム数
#define MAX_MOTION_MODEL (64)	// モーションに使えるモデル数
#define MAX_PARTS (64)			// 最大パーツ数
	/*
	全体モーション管理
		モーション(ニュートラル, 歩きなど)
			現在キー
			キーフレーム
		パーツ
		現在モーション
	*/
	//パーツデータ
	class CPartsData
	{
	public:
		CPartsData();
		virtual ~CPartsData();

		void SetPosTarget(D3DXVECTOR3 pos) { m_x.pos = pos; }	// 位置設定
		void SetRotTarget(D3DXVECTOR3 rot) { m_x.rot = rot; }	// 向き設定
		void SetSclTarget(D3DXVECTOR3 scl) { m_x.scl = scl; }	// スケール設定

		D3DXVECTOR3 GetPosTarget() { return m_x.pos; }	// 位置取得
		D3DXVECTOR3 GetRotTarget() { return m_x.rot; }	// 向き取得
		D3DXVECTOR3 GetSclTarget() { return m_x.scl; }	// スケール取得
	private:
		X m_x;	// トランスフォーム
	};

	// キーフレーム
	class CKye
	{
	public:
		CKye();
		virtual ~CKye();
		void SetFrame(int nFrame) { m_nFrame = nFrame; }	// フレーム設定

		CPartsData* GetPartsData(int nParts) { return m_pParts[nParts]; }	// パーツデータ取得
		int GetFrame() { return m_nFrame; }	// フレーム数取得
		CPartsData* createPartsData();	// パーツデータ生成

	private:
		CPartsData* m_pParts[MAX_PARTS];	// パーツ
		int m_nFrame;	// フレーム数
	};

	// モーション
	class CMotion
	{
	public:
		CMotion();
		virtual ~CMotion();
		void SetNumKye(int nNumKye) { m_nNumKye = nNumKye; }	// キーフレム数
		void SetLoop(bool bLoop) { m_bLoop = bLoop; }	// ループするか設定
		void createKye();	// キーフレーム生成

		CKye* GetKye(int nKye) { return m_pKye[nKye]; }	// キーフレーム取得
		int GetNumKye() { return m_nNumKye; }	// キーフレーム数取得
		bool GetLoop() { return m_bLoop; }	// ループ状態取得
	private:
		CKye* m_pKye[MAX_KYE];	// キーフレーム
		int m_nNumKye;	// キーフレーム数
		bool m_bLoop;	// このモーションをループするか
	};

	// 現在モーション
	class CNowMotion
	{
	public:
		CNowMotion();
		virtual ~CNowMotion();
		// 設定
		void SetMotion(int nMosyon) { m_nCntMotion = nMosyon; }		// モーションカウント
		void SetKye(int nKye) { m_nCntKye = nKye; }			// キーカウント
		void SetFrame(int nFrame) { m_nCntFrame = nFrame; }		// フレーム

		// 取得
		int GetMotion() { return m_nCntMotion; }	// モーションカウント
		int GetKye() { return m_nCntKye; }		// キーカウント
		int GetFrame() { return m_nCntFrame; }		// フレーム

		// データ追加
		void AddMotion(int nNum) { m_nCntMotion += nNum; }	// モーションカウント
		void AddKye(int nNum) { m_nCntKye += nNum; }		// キーカウント
		void AddFrame(int nNum) { m_nCntFrame += nNum; }		// フレーム

	private:
		int m_nCntMotion;	// モーションカウント
		int m_nCntKye;		// キーカウント
		int m_nCntFrame;	// フレーム
	};

	// パーツ
	class CParts : public CObjectX
	{
	public:
		CParts();
		CParts(int nPriority);
		virtual ~CParts()					override;
		virtual HRESULT Init()		override; // 初期化
		virtual void Uninit()	override; // 終了
		virtual void Update()	override; // 更新
		virtual void Draw()		override; // 描画


		// ベース位置設定
		void SetBasicX(X x) {
			m_xBasic.pos = x.pos;
			m_xBasic.rot = x.rot;
			m_xBasic.scl = x.scl;
		}
		void SetBasicX(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 siz) {
			m_xBasic.pos = pos;
			m_xBasic.rot = rot;
			m_xBasic.scl = siz;
		}
		void SetBasicPos(D3DXVECTOR3 pos) { m_xBasic.pos = pos; }	// ベース位置設定
		void SetBasicRot(D3DXVECTOR3 rot) { m_xBasic.rot = rot; }	// ベース向き設定
		void SetBasicScl(D3DXVECTOR3 scl) { m_xBasic.scl = scl; }	// ベーススケール設定

		// 追加位置
		void SetOffsetX(X x) {
			m_xOffset.pos = x.pos;
			m_xOffset.rot = x.rot;
			m_xOffset.scl = x.scl;
		}
		void SetOffsetX(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 siz) {
			m_xOffset.pos = pos;
			m_xOffset.rot = rot;
			m_xOffset.scl = siz;
		}
		void SetOffsetPos(D3DXVECTOR3 pos) { m_xOffset.pos = pos; }	// オフセット位置設定
		void SetOffsetRot(D3DXVECTOR3 rot) { m_xOffset.rot = rot; }	// オフセット向き設定
		void SetOffsetScl(D3DXVECTOR3 siz) { m_xOffset.scl = siz; }	// オフセットスケール設定

		// 動き設定
		void SetMoveX(X x, int nFrame) { SetMoveX(x.pos, x.rot, x.scl, nFrame); };
		void SetMoveX(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 siz, int nFrame);

		// データ設定
		void SetParentID(int nParent) { m_nParent = nParent; }		// パーツID設定
		void SetParentMtx(D3DXMATRIX pParent) { m_mtxParent = pParent; }	// パーツマトリクス設定
		void SetDisplay(bool bDesplay) { m_bDisplay = bDesplay; }	// 表示するかどうか

		void SetMotion(int nMotion);	// モーション設定

		// ベーストランスフォーム取得
		X GetBasicX() {
			return m_xBasic;
		}
		D3DXVECTOR3 GetBasicPos() { return m_xBasic.pos; }	// ベース位置取得
		D3DXVECTOR3 GetBasicRot() { return m_xBasic.rot; }	// ベース向き取得
		D3DXVECTOR3 GetBasicScl() { return m_xBasic.scl; }	// ベーススケール取得

		// オフセットトランスフォーム取得
		X GetOffsetX() { return m_xOffset; }
		D3DXVECTOR3 GetOffsetPos() { return m_xOffset.pos; }	// オフセット位置取得
		D3DXVECTOR3 GetOffsetRot() { return m_xOffset.rot; }	// オフセット向き取得
		D3DXVECTOR3 GetOffsetScl() { return m_xOffset.scl; }	// オフセットスケール

		// データ取得
		int GetParentID() { return m_nParent; }				// 親ID取得
		D3DXMATRIX GetParentMtx() { return m_mtxParent; }	// 親マトリクス取得
		bool GetDisplay() { return m_bDisplay; }			// 表示するかどうか
		CNowMotion* GetNowMotion() { return m_pNowMotion; }	// 現在モーション

		void SetDoMotion(bool bDoMotion) { m_bDoMotion = bDoMotion; }	// モーションするか設定

	private:

		X m_xBasic;			// 基本位置
		X m_xOffset;			// 追加位置
		X m_xMove;			// 動き

		int m_nParent;			// 親ID
		D3DXMATRIX m_mtxParent;	// 親マトリクス

		bool m_bDisplay;	// 表示するかどうか

		CNowMotion* m_pNowMotion;	// 現在モーション

		bool m_bDoMotion;	// モーションするかどうか
	};

	// モーション管理
	class CObjectMotion : public CObject
	{
	public:
		CObjectMotion();
		virtual ~CObjectMotion()		override;
		virtual HRESULT Init()		override;
		virtual void Uninit()	override;
		virtual void Update()	override;
		virtual void Draw()		override;

		// 1f前の位置設定
		virtual void SetReleaseScene(bool bRelease) override;	// シーンでリリースするか
		void SetMotion(int nMotion);	// モーション設定
		void SetMotionMove(bool bMotion) { m_bMotion = bMotion; }	// モーションを動かすかどうか設定
		virtual void SetPartsNormalUpdate(bool bUpdate);	// 全体で更新するか設定
		virtual void SetPartsPoseUpdate(bool bUpdate);		// ポーズ中更新するか設定
		virtual void SetPartsNormalDraw(bool bDraw);		// 全体で描画するか設定
		virtual void SetPartsPoseDraw(bool bDraw);			// ポーズ中描画するか設定

		CMotion* GetMotion(int nNum) { return m_pMotion[nNum]; }// モーションデータ取得
		int GetMotion() { return m_nNowMotion; }	// 現在モーション
		int GetNumParts() { return m_nParts; }	// パーツ数取得
		CParts* GetParts(int nNum) { return m_pParts[nNum]; }	// パーツ取得
		static CObjectMotion* create(const char* FileName);		// モーション生成
		static CObjectMotion* create(string FileName);			// モーション生成

	protected:
		bool Load(const char* aFileName);	// モーション読み込み

		virtual int GetNextMotion() { return 0; };	// 次取得
	private:
		void createMotion();// モーション生成
	private:
		int m_nModel;	// モデル数

		int m_nNumMotion;	// モーション数
		CMotion* m_pMotion[MAX_MOTION];	// モーションデータ

		int m_nParts;				// パーツ数
		CParts* m_pParts[MAX_PARTS];

		int m_nNextMotion;	// 次モーション
		int m_nNowMotion;	// 現在モーション

		D3DXMATRIX m_mtxWorld;	// ワールドマトリクス

		bool m_bMotion;	// モーションを動かすかどうか

		static const bool s_bMotion;	// モーションを動かすかどうかの初期設定
	};



}
#endif // !_MOTION_H_