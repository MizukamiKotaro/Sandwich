#include "Collider.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"

void Collider::CreateCollider(ColliderShape shape, ColliderType type, ColliderMask mask, bool isBeDrived)
{
	shape_ = shape;
	type_ = type;
	mask_ = mask;
	isBeDrived_ = isBeDrived;

	switch (shape)
	{
	case ColliderShape::UNKNOWN:
		break;
	case ColliderShape::CIRCLE:
		shapeCircle_ = std::make_unique<ShapeCircle>();
		break;
	case ColliderShape::BOX2D:
		shapeBox2D_ = std::make_unique<ShapeBox2D>();
		break;
	case ColliderShape::MAPCHIP2D:
		break;
	case ColliderShape::SIMPLE_LINE_Y:
		shapeSimpleLineY_ = std::make_unique<ShapeLine>();
		break;
	default:
		break;
	}
}

void Collider::AddTargetMask(ColliderMask mask)
{
	if (std::find(targetMasks_.begin(), targetMasks_.end(), mask) == targetMasks_.end()) {
		targetMasks_.push_back(mask);
	}
}

void Collider::SetCircle(const Vector3& position, const float& radius)
{
	shapeCircle_->SetParameter(position, radius);
}

void Collider::SetBox2D(const Vector3& position, const Vector3& scale)
{
	shapeBox2D_->SetParameter(position, scale);
}

void Collider::SetSimpleLineY(const float& y)
{
	shapeSimpleLineY_->SetParameter(y);
}
