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

	Vector2 min2 = { aBox->position_.x - aBox->moveVector_.x - aBox->scale_.x,aBox->position_.y - aBox->moveVector_.y - aBox->scale_.y };
	Vector2 max2 = { aBox->position_.x - aBox->moveVector_.x + aBox->scale_.x,aBox->position_.y - aBox->moveVector_.y + aBox->scale_.y };
	Vector2 min3 = { bBox->position_.x - bBox->moveVector_.x - bBox->scale_.x,bBox->position_.y - bBox->moveVector_.y - bBox->scale_.y };
	Vector2 max3 = { bBox->position_.x - bBox->moveVector_.x + bBox->scale_.x,bBox->position_.y - bBox->moveVector_.y + bBox->scale_.y };

	if (min0.x > min2.x) {
		min0.x = min2.x;
	}
	if (min0.y > min2.y) {
		min0.y = min2.y;
	}
	if (max0.x < max2.x) {
		max0.x = max2.x;
	}
	if (max0.y < max2.y) {
		max0.y = max2.y;
	}

	if (min1.x > min3.x) {
		min1.x = min3.x;
	}
	if (min1.y > min3.y) {
		min1.y = min3.y;
	}
	if (max1.x < max3.x) {
		max1.x = max3.x;
	}
	if (max1.y < max3.y) {
		max1.y = max3.y;
	}

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}
