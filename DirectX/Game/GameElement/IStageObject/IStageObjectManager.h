#pragma once
#include <string>
#include <list>
#include <memory>
class IStageObject;
class StageEditor;
class Camera;
enum class StageObjectType;
class DescriptorHandles;

class IStageObjectManager {
public:
	void AddType(const StageObjectType& type, const std::string& mainName, const std::string& subName);
	void Clear() { objects_.clear(); }
	void Initialize();
	void Update(const float& deltaTime);
	void Draw(const Camera& camera) const;
	void DrawLight(const Camera& camera) const;

protected:
	void ApplyGlobalVariable();
	void CreateObjects();

	std::list<std::unique_ptr<IStageObject>> objects_;
	std::unique_ptr<StageEditor> stageEditor_;

	struct Type
	{
		uint32_t num;
		uint32_t preNum;
		StageObjectType type;
		std::string mainName;
		std::string subName;
	};
	std::list<std::unique_ptr<Type>> types_;
};