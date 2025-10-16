//===========================================
// 
// シーンベース[scene_base.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _SCENE_H_
#define _SCENE_H_
namespace My
{
	namespace Scene {
		class CCommonData;	// 共有データ
		class CBase {		// ベースクラス
		protected:
			// メインループ前にのみ使用するコンストラクタ
			CBase(CCommonData* const commonData) :
				m_pCommonData(commonData)
			{}
			// 派生クラスのコンストラクタで使用するコンストラクタ
			CBase(const CBase&) = default;

		public:
			//コピー禁止
			CBase& operator=(const CBase&) = delete;

			// 仮想関数化したデストラクタ
			virtual ~CBase() = default;

			virtual CBase* Update() = 0;	// 更新
			virtual void Draw() const = 0;	// 描画

			virtual bool GetPose() = 0;

			//実体生成用関数
			template<typename T>
			CBase* makeScene();
		protected:

			CCommonData* const m_pCommonData;	// 共有データ

		private:
		};
	}
}
#endif // _SCENE_H_