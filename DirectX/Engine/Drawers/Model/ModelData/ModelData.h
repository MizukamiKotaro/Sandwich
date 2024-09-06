#pragma once
#include "MeshData3D.h"
#include "NodeData3D.h"
#include "AnimationData3D.h"
#include "BoneData3D.h"
#include "Texture.h"
#include "WeightData.h"

class ModelData
{
public:
	MeshData mesh;
	NodeData rootNode;
	BoneData boneData;
	AnimationData animationData;
	std::string fileName;
	const Texture* texture;
	std::map<std::string, JointWeightData> skinClusterData;
};