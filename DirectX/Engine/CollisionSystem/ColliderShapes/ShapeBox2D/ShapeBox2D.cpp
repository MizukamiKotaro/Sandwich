#include "ShapeBox2D.h"

ShapeBox2D::ShapeBox2D()
{
	position_ = {};
	scale_ = {};
	moveVector_ = {};
}

void ShapeBox2D::SetParameter(const Vector3& position, const Vector3& scale, const Vector3& moveVector)
{
	position_ = position;
	scale_ = scale;
	moveVector_ = moveVector;
}
