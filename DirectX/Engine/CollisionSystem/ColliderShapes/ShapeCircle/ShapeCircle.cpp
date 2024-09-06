#include "ShapeCircle.h"

ShapeCircle::ShapeCircle()
{
	position_ = {};
	radius_ = 1.0f;
}


void ShapeCircle::SetParameter(const Vector3& position, const float& radius)
{
	position_ = position;
	radius_ = radius;
}
