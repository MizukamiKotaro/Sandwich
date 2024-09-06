#pragma once

#include <vector>
#include <string>
#include <map>
#include <optional>
#include "Transform/QuaternionTransform.h"

struct VertexWeight {
	uint32_t vertexID;
	float weight;
};

struct BoneData
{
	std::string name;
	Matrix4x4 offsetMatrix;
	std::vector<VertexWeight> vertexWeights;
};

struct Joint
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix; // skeletonSpeceでの変換行列
	std::string name;
	std::vector<int32_t> children; // 子jointのIndexのリスト
	int32_t index;
	std::optional<int32_t> parent;
};

struct Skeleton
{
	int32_t root; // RootJointのIndex
	std::map<std::string, int32_t> jointMap; // Joint名とIndex
	std::vector<Joint> joints;
};