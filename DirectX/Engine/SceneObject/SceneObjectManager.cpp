#include "SceneObjectManager.h"
#include "LevelDataLoader/LevelData/LevelDataManager.h"
#include "Model.h"
SceneObjectManager* SceneObjectManager::GetInstance()
{
	static SceneObjectManager instance;
	return &instance;
}

void SceneObjectManager::CreateObjects(const std::string& sceneName)
{
	const LevelData& data = LevelDataManager::GetInstance()->GetLevelData(sceneName);
	objects_.clear();
	CreateObjects(data.objects);
}

void SceneObjectManager::Update()
{
	for (std::unique_ptr<BaseModel>& obj : objects_) {
		obj->Update();
	}
}

void SceneObjectManager::Draw(const Camera& camera) const 
{
	for (const std::unique_ptr<BaseModel>& obj : objects_) {
		obj->Draw(camera);
	}
}

void SceneObjectManager::CreateObjects(const std::vector<LevelData::ObjectData>& datas, const BaseModel* parent)
{
	for (const LevelData::ObjectData& data : datas) {
		objects_.push_back(std::make_unique<Model>(data.fileName));
		objects_.back()->transform_.translate_ = data.translation;
		objects_.back()->transform_.rotate_ = data.rotation;
		objects_.back()->transform_.scale_ = data.scaling;

		if (parent) {
			objects_.back()->transform_.parent_ = &parent->transform_;
		}
		objects_.back()->Update();
		if (data.children.size() != 0) {
			CreateObjects(data.children, objects_.back().get());
		}
	}
}

