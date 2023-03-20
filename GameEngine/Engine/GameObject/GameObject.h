#pragma once
#include<string>
#include<list>
#include"../Collider/BoxCollider.h"
#include"../Collider/SphereCollider.h"
#include"../Collider/OBBCollider.h"
#include"Transform.h"

class GameObject
{
	friend class Collider;
protected:
	std::list<GameObject*> childList_;	//�q���X�g
	std::list<Collider*> colliderList_; //�R���C�_�[���X�g
	Transform	transform_;				//�I�u�W�F�N�g�̏��
	GameObject* pParent_;				//�e�I�u�W�F�N�g�̏��
	std::string	objectName_;			//���O
	int objectID_;						//�I�u�W�F�N�g�ŗL�̔ԍ�
	std::string objectTag_;				//�I�u�W�F�N�g�ɔC�ӂł����鎯�ʗp�̖��O
	bool killFlag_;						//�L�����邩�ǂ���
	bool activeFlag_;					//�I�u�W�F�N�g���A�N�e�B�u(Update�Ă΂��)���ǂ���
	bool startFlag_;					//�����A�N�e�B�u�ɂȂ��ĂȂ��ꍇfalse
	bool drawFlag_;						//�`�悷�邩�ǂ���

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void BeforeDeath() {};
	virtual void Draw() {};
	virtual void SecondDraw() {};
	virtual void ThirdDraw() {};
	virtual void Release() = 0;

	void UpdateSub();
	void FixedUpdateSub();
	void DrawSub();
	void SecondDrawSub();
	void ThirdDrawSub();
	void ReleaseSub();

	template<class T>
	T* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		if (parent != nullptr)
		{
			parent->PushBackChild(p);
		}
		return p;
	}

	void KillMe() { killFlag_ = true; }
	void IsDraw(bool flag) { drawFlag_ = flag; }
	bool GetDrawFlag() { return drawFlag_; }

	///////////////////////////�Փˊ֘A�̊֐�////////////////////////
	virtual void OnCollision(GameObject* pTarget) {};
			void Collision(GameObject* pTarget);
			void AddCollider(Collider* collider);
			void DelCollider(const GameObject& obj);



			void KillAllChildren();
			void KillObjectSub(GameObject* pTarget);
			void PushBackChild(GameObject* pTarget);

	///////////////////////////�e�Z�b�^�[�Q�b�^�[//////////////////////////////////////
			void SetPositionX(float x) { transform_.position_.x = x; }
			void SetPositionY(float y) { transform_.position_.y = y; }
			void SetPositionZ(float z) { transform_.position_.z = z; }
			void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }

			void SetRotateX(float x) { transform_.rotate_.x = x; }
			void SetRotateY(float y) { transform_.rotate_.y = y; }
			void SetRotateZ(float z) { transform_.rotate_.z = z; }
			void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }

			void SetScaleX(float x) { transform_.scale_.x = x; }
			void SetScaleY(float y) { transform_.scale_.y = y; }
			void SetScaleZ(float z) { transform_.scale_.z = z; }
			void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }

			void SetTag(std::string tagName) { objectTag_ = tagName; }
			void SetActive(bool status) { activeFlag_ = status; }
			std::string GetTag() { return objectTag_; }

			Transform GetTransform();
			XMFLOAT3  GetPosition();
			XMFLOAT3  GetRotate();
			XMFLOAT3  GetScale();
			std::list<GameObject*>* GetChildList() { return &childList_; }
			std::string GetObjectName() { return objectName_; }
			int GetObjectID() { return objectID_; }

			//�w�肵�����W�ɉ�]������s������֐�
			XMMATRIX LookAtMatrix(XMFLOAT3 target, XMVECTOR frontVec, XMVECTOR upVec = XMVectorSet(0, 1, 0, 0));

	//�e�I�u�W�F�N�g�擾
	GameObject* GetParent();
	GameObject* GetRootJob();
	GameObject* FindObject(std::string name);
	GameObject* FindChild(std::string name);
	XMMATRIX    GetWorldMatrix();



	//���[���h���W�擾
	XMFLOAT3    GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }

};