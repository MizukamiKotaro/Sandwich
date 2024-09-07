#include "CollisionBox2DLine.h"
#include "CollisionSystem/Collider/Collider.h"
#include <cmath>

bool CollisionBox2DLine::IsCollision(Collider* a, Collider* b)
{
	ShapeBox2D* aBox = a->GetBox2D();
	float  y = b->GetLine()->y_;

	if (std::fabsf(aBox->position_.y - y) <= aBox->scale_.y) {
		return true;
	}
	return false;
}
