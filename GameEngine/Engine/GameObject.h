#pragma once
#include<string>
#include<list>
#include"BoxCollider.h"
#include"SphereCollider.h"
#include"Transform.h"

class GameObject
{
	friend class Collider;
protected:
	std::list<GameObject*> childList_;	//子リスト
	std::list<Collider*> colliderList_; //コライダーリスト
	Transform	transform_;				//オブジェクトの情報
	GameObject* pParent_;				//親オブジェクトの情報
	std::string	objectName_;			//名前
	bool killFlag;						//キルするかどうか

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void Draw()       = 0;
	virtual void Release()    = 0;

	void UpdateSub();
	void FixedUpdateSub();
	void DrawSub();
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
		p->Initialize();
		return p;
	}

	void KillMe()
	{
		killFlag = true;
	}

	///////////////////////////追加分////////////////////////
	virtual void OnCollision(GameObject* pTarget) {};
			void Collision(GameObject* pTarget);
			void AddCollider(Collider* collider);
			void KillAllChildren();
			void KillObjectSub(GameObject* pTarget);
			void PushBackChild(GameObject* pTarget);

	//親オブジェクト取得
	GameObject* GetParent();
	GameObject* GetRootJob();
	GameObject* FindObject(std::string name);
	GameObject* FindChild(std::string name);
	XMMATRIX    GetWorldMatrix();



	//ワールド座標取得
	XMFLOAT3    GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }

};