#pragma once
#include "RigidAnimationModel/RigidAnimationModel.h"
#include "SpotLight/SpotLight.h"
#include "PointLight/PointLight.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;

class SpotLightBoxAnimation {
public:
	SpotLightBoxAnimation(const std::string& name);
	void Initialize();
	void Update(const float& time);
	void Draw(Camera* camera);
	void DrawLight(const Camera& camera);
private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void LightUpdate();

public:
	std::unique_ptr<SpotLight> spotLights_[2];
	std::unique_ptr<RigidAnimationModel> box_;
	bool isDraw_ = true;
private:
	std::unique_ptr<PointLight> point_;
	std::unique_ptr<GlobalVariableUser> global_;
};