#include "TwinSpotLightBoxAnimation.h"
#include "Camera.h"
#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

TwinSpotLightBoxAnimation::TwinSpotLightBoxAnimation()
{
	lights_[0] = std::make_unique<SpotLightBoxAnimation>("a");
	lights_[1] = std::make_unique<SpotLightBoxAnimation>("b");
}

void TwinSpotLightBoxAnimation::Initialize()
{
}

void TwinSpotLightBoxAnimation::Update()
{
	*lights_[1]->spotLights_[0]->light_ = *lights_[0]->spotLights_[0]->light_;
	lights_[1]->spotLights_[0]->light_->direction.x *= -1;
	lights_[1]->box_->transform_ = lights_[0]->box_->transform_;
	lights_[1]->box_->transform_.translate_.x *= -1;

	lights_[0]->Update(0);
	lights_[1]->Update(0);
}

void TwinSpotLightBoxAnimation::Draw(Camera* camera)
{
	lights_[0]->Draw(camera);
	lights_[1]->Draw(camera);
}
