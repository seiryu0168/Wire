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
	std::list<GameObject*> childList_;	//子リスト
	std::list<Collider*> colliderList_; //コライダーリスト
	Transform	transform_;				//オブジェクトの情報
	GameObject* pParent_;				//親オブジェクトの情報
	std::string	objectName_;			//名前
	int objectID_;						//オブジェクト固有の番号
	std::string objectTag_;				//オブジェクトに任意でつけられる識別用の名前
	bool killFlag_;						//キルするかどうか
	bool activeFlag_;					//オブジェクトがアクティブ(Update呼ばれる)かどうか
	bool startFlag_;					//一回もアクティブになってない場合false
	bool drawFlag_;						//描画するかどうか

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

	///////////////////////////衝突関連の関数////////////////////////
	virtual void OnCollision(GameObject* pTarget) {};
			void Collision(GameObject* pTarget);
			void AddCollider(Collider* collider);
			void DelCollider(const GameObject& obj);



			void KillAllChildren();
			void KillObjectSub(GameObject* pTarget);
			void PushBackChild(GameObject* pTarget);

	///////////////////////////各セッターゲッター//////////////////////////////////////
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

			//指定した座標に回転させる行列を作る関数
			XMMATRIX LookAtMatrix(XMFLOAT3 target, XMVECTOR frontVec, XMVECTOR upVec = XMVectorSet(0, 1, 0, 0));

	//親オブジェクト取得
	GameObject* GetParent();
	GameObject* GetRootJob();
	GameObject* FindObject(std::string name);
	GameObject* FindChild(std::string name);
	XMMATRIX    GetWorldMatrix();



	//ワールド座標取得
	XMFLOAT3    GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }

};