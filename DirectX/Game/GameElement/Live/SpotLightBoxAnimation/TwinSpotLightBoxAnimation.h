#pragma once
#include "SpotLightBoxAnimation.h"
#include <array>
#include <memory>

class Camera;

class TwinSpotLightBoxAnimation {
public:
	TwinSpotLightBoxAnimation();
	void Initialize();
	void Update();
	void Draw(Camera* camera);

public:
	std::array<std::unique_ptr<SpotLightBoxAnimation>, 2> lights_;
};