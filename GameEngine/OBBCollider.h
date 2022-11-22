#pragma once
#include"Engine/Collider.h"
class OBBCollider : public Collider
{
private:
	friend class Collider;
	XMVECTOR OBB_X;
	XMVECTOR OBB_Y;
	XMVECTOR OBB_Z;
public:
	OBBCollider(XMFLOAT3 size, XMFLOAT3 basePos = {0,0,0}, bool isRotate = true, bool isScale = true);
	bool isHit(Collider* target);


};

