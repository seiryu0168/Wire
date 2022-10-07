#include "SphereCollider.h"

SphereCollider::SphereCollider(XMFLOAT3 basePos, float radius)
{
	center_ = basePos;
	size_ = XMFLOAT3(radius, radius, radius);
	type_ = SPHERE_COLLIDER;
}


bool SphereCollider::IsHit(Collider* target)
{
	if (target->type_ == BOX_COLLIDER)
	{
		return IsHitBox_Sphere((BoxCollider*)target, this);
	}
	else
		return IsHitSphere_Sphere((SphereCollider*)target, this);
}