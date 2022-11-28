#include "OBBCollider.h"

OBBCollider::OBBCollider(XMFLOAT3 size, XMFLOAT3 basePos = { 0,0,0 }, bool isRotate = true, bool isScale = true)
{

}
bool OBBCollider::IsHit(Collider* target)
{
	switch (target->type_)
	{
	case OBB_COLLIDER:
		return IsHitOBB_OBB(this, (OBBCollider*)target);
	}
}