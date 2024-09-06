#include "CollisionCircleCircle.h"
#include "CollisionSystem/Collider/Collider.h"
#include <cmath>

bool CollisionCircleCircle::IsCollision(Collider* a, Collider* b)
{
	ShapeCircle* aCircle = a->GetCircle();
	ShapeCircle* bCircle = b->GetCircle();

	if ((aCircle->position_ - bCircle->position_).Length() <= aCircle->radius_ + bCircle->radius_) {
		if (a->GetType() == ColliderType::COLLIDER || b->GetType() == ColliderType::COLLIDER) {
			return true;
		}
	}
	return false;
}
