#pragma once
#include "Vector3.h"
class ShapeBox2D {
public:
	ShapeBox2D();
	void SetParameter(const Vector3& position, const Vector3& scale, const Vector3& moveVector);

public:
	Vector3 position_;
	Vector3 scale_;
	Vector3 moveVector_;
};