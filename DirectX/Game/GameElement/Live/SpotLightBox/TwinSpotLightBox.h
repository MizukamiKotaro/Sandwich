#pragma once
#include "SpotLightBox.h"
#include <array>
#include <memory>

class Camera;

class TwinSpotLightBox {
public:
	TwinSpotLightBox();
	void Initialize();
	void Update();
	void Draw(Camera* camera);

public:
	std::array<std::unique_ptr<SpotLightBox>, 2> lights_;
};