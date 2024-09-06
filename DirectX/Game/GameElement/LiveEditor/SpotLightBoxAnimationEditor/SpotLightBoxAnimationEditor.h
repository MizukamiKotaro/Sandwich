#pragma once
#include "RigidAnimationModel/RigidAnimationModel.h"
#include "SpotLight/SpotLight.h"
#include "PointLight/PointLight.h"
#include "GameElement/IStageObject/IStageObject.h"

class Camera;

class SpotLightBoxAnimationEditor : public IStageObject
{
public:
	SpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const uint32_t& no);
	SpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const std::string& treeName, const uint32_t& no);
	SpotLightBoxAnimationEditor();
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;
	void DrawLight(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;

	void LightUpdate();
	void Create();

public:
	std::unique_ptr<SpotLight> spotLights_[2];
	std::unique_ptr<RigidAnimationModel> box_;
	bool isDraw_ = true;
private:
	std::unique_ptr<PointLight> point_;

};