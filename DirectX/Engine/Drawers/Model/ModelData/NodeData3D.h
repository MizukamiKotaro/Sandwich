#pragma once

#include <vector>
#include <string>
#include "Transform/QuaternionTransform.h"

class NodeData
{
public:
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<NodeData> children;
};