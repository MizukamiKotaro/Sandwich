#pragma once
#include <string>
#include <vector>
#include "Vector3.h"

class LevelData
{
public:
	class ObjectData
	{
	public:
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		std::vector<ObjectData> children;
	};

	LevelData();
	

public:
	std::vector<ObjectData> objects;
};