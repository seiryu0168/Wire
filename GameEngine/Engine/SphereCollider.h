#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
private:
	friend class Collider;
public:
	SphereCollider(XMFLOAT3 basePos, float radius);

	bool IsHit(Collider* target) override;
};

