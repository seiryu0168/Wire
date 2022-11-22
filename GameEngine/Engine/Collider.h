#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include"Transform.h"
using namespace DirectX;

//�O���錾
class GameObject;
class BoxCollider;
class SphereCollider;

enum ColliderType
{
	BOX_COLLIDER,
	SPHERE_COLLIDER,
};

//�����蔻��
class Collider
{

	friend class BoxCollider;
	friend class SphereCollider;

	GameObject*  pColObject_;	//�����蔻���t����I�u�W�F�N�g
	ColliderType type_;		//�R���C�_�[�^�C�v
	XMFLOAT3     center_;		//���_
	XMFLOAT3     size_;			//�傫��
	Transform    transform_;


public:
	Collider();
	virtual ~Collider();
	//�Փ˔���
	virtual bool IsHit(Collider* target) = 0;

	//�Փ˔͈�
	
	//���^�Ɣ��^
	//������BoxCollider�^��BoxCollider�^
	bool IsHitBox_Box(BoxCollider* box1, BoxCollider* box2);

	//���^�Ƌ��^
	//������BoxCollider�^��SphereCollider�^
	bool IsHitBox_Sphere(BoxCollider* box, SphereCollider* sphere);

	//���^�Ƌ��^
	//������SphereCollider�^��SphereCollider�^
	bool IsHitSphere_Sphere(SphereCollider* sphereA, SphereCollider* sphereB);

	bool IsHitBox_Mesh(BoxCollider* Box, int modelNum);

	//�I�u�W�F�N�g���Z�b�g
	void SetGemaObject(GameObject* gameObject) { pColObject_ = gameObject; }


	void SetCollider(XMFLOAT3 center,XMFLOAT3 size)
	{
		center_ = center;
		size_ = size;
	}
};

