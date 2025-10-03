//=============================================
//
//�I�u�W�F�N�g[object.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _OBJECT_H_
#include "main.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �I�u�W�F�N�g�N���X */
	class CObject
	{
	public:
		static constexpr int MAX_OBJECT = 8192; //�I�u�W�F�N�g�̍ő吔

		static constexpr int PRI_MAX = 30; //�v���C�I���e�B�̍ő�l

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

		static void ReleaseAll(); //�S�I�u�W�F�N�g���

		static void UpdateAll(); //�S�I�u�W�F�N�g�X�V

		static void DrawAll(); //�S�I�u�W�F�N�g�`��


		/**
		 * @brief �擪�̃I�u�W�F�N�g�擾
		 * @param [in]�`��D��x
		 * @return �擪�I�u�W�F�N�g
		 */
		static CObject* GetTopObject(int priority);

		/**
		 * @brief ���̃I�u�W�F�N�g�擾
		 * @return ���̃I�u�W�F�N�g
		 */
		CObject* GetNext() { return m_pNext; }

		/**
		 * @brief ����
		 * @param [in]���X�g
		 */
		template<class T>
		void Search(std::list<T*>& list)
		{
			//�v���C�I���e�B�̐���������
			for (int i = 0; i < PRI_MAX; i++)
			{
				CObject* pObj = m_pTop[i];	//�擪�擾

				//�ő吔���s���Ȃ̂�while���g�p
				while (pObj != nullptr)
				{
					CObject* pNext = pObj->m_pNext;	//���̃|�C���^���擾

					//�L���X�g�����������烊�X�g�ɑ}��
					T* pListObj = dynamic_cast<T*>(pObj);
					if (pListObj != nullptr)
					{
						list.push_back(pListObj);
					}

					pObj = pNext;					//�|�C���^��i�߂�
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

		//�I�u�W�F�N�g�̏������~�߂邩��ݒ�
		inline void SetStop(bool isStop) 
		{ 
			m_isStop = isStop; 
		}

		//�I�u�W�F�N�g�̏������~�߂邩���擾
		inline bool GetStop() 
		{ 
			return m_isStop; 
		}

	protected:
		void Release(); //�������g�̉��

		inline bool& GetisDraw()
		{
			return m_isDraw;
		}

		inline void SetType(OBJECT_TYPE type)
		{
			m_type = type;
		}

	private:
		static CObject* m_pTop[PRI_MAX];	//�擪�̃I�u�W�F�N�g�|�C���^
		static CObject* m_pCur[PRI_MAX];	//���݁i�Ō���j�̃|�C���^
		CObject* m_pPrev;						//�O�̃I�u�W�F�N�g�̃|�C���^
		CObject* m_pNext;						//���̃I�u�W�F�N�g�̃|�C���^
		int m_nPriority; //�`��D��x

		static int m_nNumAll; //����

		bool m_isStop;	//�������~�߂�

		bool m_isDelete;	//�������ǂ���

		bool m_isDraw;		//�`�悷�邩

		int m_nID; //�I�u�W�F�N�g��ID
		OBJECT_TYPE m_type;

	};
}
#endif