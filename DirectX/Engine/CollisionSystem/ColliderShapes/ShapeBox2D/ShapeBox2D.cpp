#include "ShapeBox2D.h"

ShapeBox2D::ShapeBox2D()
{
	position_ = {};
	scale_ = {};
}

void ShapeBox2D::SetParameter(const Vector3& position, const Vector3& scale)
{
	position_ = position;
	scale_ = scale;
}
