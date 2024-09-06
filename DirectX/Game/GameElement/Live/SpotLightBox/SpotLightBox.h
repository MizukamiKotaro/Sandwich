#pragma once
#include "Model.h"
#include "SpotLight/SpotLight.h"
#include "PointLight/PointLight.h"

class Camera;

class SpotLightBox {
public:
	SpotLightBox();
	void Initialize();
	void Update();
	void Draw(Camera* camera);
	void DrawLight(const Camera& camera);

public:
	std::unique_ptr<SpotLight> spotLights_[2];
	std::unique_ptr<Model> box_;
	bool isDraw_ = true;
private:
	std::unique_ptr<PointLight> point_;
};