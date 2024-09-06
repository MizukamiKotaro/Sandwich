#include "CollisionCircleLine.h"
#include "CollisionSystem/Collider/Collider.h"
#include <cmath>

bool CollisionCircleLine::IsCollision(Collider* a, Collider* b)
{
	ShapeCircle* aCircle = a->GetCircle();
	float  y = b->GetLine()->y_;

	if (std::fabsf(aCircle->position_.y - y) <= aCircle->radius_) {
		return true;
	}
	return false;
}
