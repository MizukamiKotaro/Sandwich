#include "CollisionBox2DBox2D.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Vector2.h"

bool CollisionBox2DBox2D::IsCollision(Collider* a, Collider* b)
{
	ShapeBox2D* aBox = a->GetBox2D();
	ShapeBox2D* bBox = b->GetBox2D();

	Vector2 min0 = { aBox->position_.x - aBox->scale_.x,aBox->position_.y - aBox->scale_.y };
	Vector2 max0 = { aBox->position_.x + aBox->scale_.x,aBox->position_.y + aBox->scale_.y };
	Vector2 min1 = { bBox->position_.x - bBox->scale_.x,bBox->position_.y - bBox->scale_.y };
	Vector2 max1 = { bBox->position_.x + bBox->scale_.x,bBox->position_.y + bBox->scale_.y };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}
