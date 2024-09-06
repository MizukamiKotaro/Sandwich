#pragma once
#include <memory>
#include <list>
#include <string>
#include <vector>
#include "LevelDataLoader/LevelData/LevelData.h"
class BaseModel;
class Camera;
class SceneObjectManager
{
public:
	static SceneObjectManager* GetInstance();

	void CreateObjects(const std::string& sceneName);
	void Update();
	void Draw(const Camera& camera) const;

private:
	SceneObjectManager() = default;
	~SceneObjectManager() = default;
	SceneObjectManager(const SceneObjectManager& obj) = delete;
	SceneObjectManager& operator=(const SceneObjectManager& obj) = delete;

	void CreateObjects(const std::vector<LevelData::ObjectData>& datas, const BaseModel* parent = nullptr);
private:
	std::list<std::unique_ptr<BaseModel>> objects_;
};