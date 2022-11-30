#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include"Transform.h"
using namespace DirectX;

//前方宣言
class GameObject;
class BoxCollider;
class SphereCollider;
class OBBCollider;

enum ColliderType
{
	BOX_COLLIDER,
	SPHERE_COLLIDER,
	OBB_COLLIDER
};

//当たり判定
class Collider
{

	friend class BoxCollider;
	friend class SphereCollider;
	friend class OBBCollider;

	GameObject*  pColObject_;	//当たり判定を付けるオブジェクト
	ColliderType type_;		//コライダータイプ
	XMFLOAT3     center_;		//原点
	XMFLOAT3     size_;			//大きさ
	Transform    transform_;


public:
	Collider();
	virtual ~Collider();
	//衝突判定
	virtual bool IsHit(Collider* target) = 0;

	//衝突範囲
	
	//箱型と箱型
	//引数はBoxCollider型とBoxCollider型
	bool IsHitBox_Box(BoxCollider* box1, BoxCollider* box2);

	//箱型と球型
	//引数はBoxCollider型とSphereCollider型
	bool IsHitBox_Sphere(BoxCollider* box, SphereCollider* sphere);

	//球型と球型
	//引数はSphereCollider型とSphereCollider型
	bool IsHitSphere_Sphere(SphereCollider* sphereA, SphereCollider* sphereB);

	//OBB同士の衝突判定
	bool IsHitOBB_OBB(OBBCollider* obbA,OBBCollider* obbB);

	//コライダーを付けたオブジェクトを返す
	GameObject* GetpColObject() { return pColObject_; }
	
	//オブジェクトをセット
	void SetGemaObject(GameObject* gameObject) { pColObject_ = gameObject; }


	void SetCollider(XMFLOAT3 center,XMFLOAT3 size)
	{
		center_ = center;
		size_ = size;
	}
};

