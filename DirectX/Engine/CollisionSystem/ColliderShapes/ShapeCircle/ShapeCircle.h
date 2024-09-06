#pragma once
#include "Vector3.h"
class ShapeCircle {
public:
	ShapeCircle();
	void SetParameter(const Vector3& position, const float& radius);

public:
	Vector3 position_;
	float radius_;
};