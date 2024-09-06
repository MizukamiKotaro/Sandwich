#pragma once
#include <memory>
#include "Drawers/IDrawer/IDrawer.h"
#include "Editor/StageEditor.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"

class Camera;
class Texture;

class IParticle : public IDrawer
{
public:
	virtual void Initialize() = 0;

	virtual void Update(const float& deltaTime) = 0;

	virtual void Draw(const Camera& camera, const BlendMode& blend = BlendMode::kBlendModeNormal) = 0;

	const Texture* GetTexture() const;

protected:
	void CreateParticle(const std::string& particleName, const std::string& textureName, bool isStageEditor = false);

	virtual void SetGlobalVariable() = 0;
	virtual void ApplyGlobalVariable() = 0;

protected:

	std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

	const Texture* texture_;
};