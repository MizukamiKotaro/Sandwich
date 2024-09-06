#include "IStageObjectManager.h"
#include "Editor/StageEditor.h"
#include "StageObjectConfig.h"
#include "StageObjectFactory/StageObjectFactory.h"
#include "IStageObject.h"

void IStageObjectManager::AddType(const StageObjectType& type, const std::string& mainName, const std::string& subName)
{
	bool is = false;
	for (const std::unique_ptr<IStageObjectManager::Type>& aType : types_) {
		if (aType->type == type && aType->mainName == mainName && aType->subName == subName) {
			is = true;
			break;
		}
	}
	if (!is) {
		types_.push_back(std::make_unique<IStageObjectManager::Type>(0, 0, type, mainName, subName));
		if (!stageEditor_) {
			stageEditor_ = std::make_unique<StageEditor>();
		}
		stageEditor_->AddItem(mainName + "の数", 0, mainName + "の設置", subName);
		types_.back()->num = stageEditor_->GetIntValue(mainName + "の数", mainName + "の設置", subName);
		CreateObjects();
	}
}

void IStageObjectManager::Initialize()
{
	if (!stageEditor_) {
		stageEditor_ = std::make_unique<StageEditor>();
	}
	stageEditor_->Initialize();
	ApplyGlobalVariable();
}

void IStageObjectManager::Update(const float& deltaTime)
{
#ifdef _DEBUG
	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
	ApplyGlobalVariable();
#endif // _DEBUG

	for (std::unique_ptr<IStageObject>& object : objects_) {
		object->Update(deltaTime);
	}
}

void IStageObjectManager::Draw(const Camera& camera) const
{
	for (const std::unique_ptr<IStageObject>& object : objects_) {
		object->Draw(camera);
	}
}

void IStageObjectManager::DrawLight(const Camera& camera) const
{
	for (const std::unique_ptr<IStageObject>& object : objects_) {
		object->DrawLight(camera);
	}
}

void IStageObjectManager::ApplyGlobalVariable()
{
	bool change = false;
	for (std::unique_ptr<IStageObjectManager::Type>& type : types_) {
		type->num = stageEditor_->GetIntValue(type->mainName + "の数", type->mainName + "の設置", type->subName);
		if (type->num != type->preNum) {
			change = true;
			type->preNum = type->num;
		}
	}

	if (change) {
		CreateObjects();
	}
}

void IStageObjectManager::CreateObjects()
{
	objects_.clear();
	uint32_t no = 0;
	for (const std::unique_ptr<IStageObjectManager::Type>& type : types_) {
		for (uint32_t i = 0; i < type->num; i++) {
			objects_.push_back(nullptr);
			objects_.back().reset(StageObjectFactory::CreateStageObject(type->type, type->mainName, type->subName, no));
			no++;
		}
		no = 0;
	}
}
