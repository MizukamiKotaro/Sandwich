#pragma once
#include "Externals/nlohmann/json.hpp"
#include "Engine/LevelDataLoader/LevelData/LevelData.h"
#include <string>

class LevelDataLoader 
{
public:
	static LevelDataLoader* GetInstance();

	void LoadFiles();

	void LoadFile(const std::string& filePath);

private:
	LevelDataLoader() = default;
	~LevelDataLoader() = default;
	LevelDataLoader(const LevelDataLoader& obj) = delete;
	LevelDataLoader& operator=(const LevelDataLoader& obj) = delete;

	void ParseRecursive(nlohmann::json& deserialized, std::vector<LevelData::ObjectData>& objects, const std::string& name = "objects");
};