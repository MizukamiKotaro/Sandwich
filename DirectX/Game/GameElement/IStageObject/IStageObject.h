#pragma once
#include "Editor/StageEditor.h"
class Camera;

class IStageObject
{
public:
	virtual ~IStageObject() = default;
	virtual void Initialize();
	virtual void Update(const float& deltaTime = 0) = 0;
	virtual void Draw(const Camera& camera) = 0;
	virtual void DrawLight(const Camera& camera);
	virtual void DrawSub(const Camera& camera);

protected:
	void CreateStageEditor(const std::string& mainName, const std::string& name, const uint32_t& no, const bool& isMany = false);
	void CreateStageEditor(const std::string& mainName, const std::string& name, const std::string& treeName, const uint32_t& no, const bool& isMany = false);
	void InitializeEditor();
	void UpdateGlobalVariable();
	virtual void SetGlobalVariable() = 0;
	virtual void ApplyGlobalVariable() = 0;

	std::unique_ptr<StageEditor> stageEditor_;
};