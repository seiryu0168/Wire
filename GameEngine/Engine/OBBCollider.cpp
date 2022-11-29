#include "OBBCollider.h"

OBBCollider::OBBCollider(XMFLOAT3 size, bool isRotate, bool isScale)
{
	rotateFlag_ = isRotate;
	scaleFlag_ = isScale;

	center_ = { 0,0,0 };
	size_ = size;
	OBB_X = XMVectorSet(1, 0, 0, 0) * size.x;
	OBB_Y = XMVectorSet(0, 1, 0, 0) * size.y;
	OBB_Z = XMVectorSet(0, 0, 1, 0) * size.z;
}

bool OBBCollider::IsHit(Collider* target)
{
	switch (target->type_)
	{
	case OBB_COLLIDER:
		return IsHitOBB_OBB(this, (OBBCollider*)target);
	}
	return false;
}

XMVECTOR OBBCollider::calcDistance(XMFLOAT3 posA, XMFLOAT3 posB)
{
	XMVECTOR distance = XMLoadFloat3(&posB) - XMLoadFloat3(&posA);
	return distance;
}

float prjLine(XMVECTOR* sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = nullptr)
{
	*sep = XMVector3Normalize(*sep);
	float r1 = fabs(XMVectorGetX(XMVector3Dot(*sep, *e1)));
	float r2 = fabs(XMVectorGetX(XMVector3Dot(*sep, *e2)));
	float r3 = e3 ? fabs(XMVectorGetX(XMVector3Dot(*sep, *e3))) : 0;

	return r1 + r2 + r3;
}