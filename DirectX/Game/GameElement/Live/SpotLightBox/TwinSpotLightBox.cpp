#include "TwinSpotLightBox.h"
#include "Camera.h"
#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

TwinSpotLightBox::TwinSpotLightBox()
{
	lights_[0] = std::make_unique<SpotLightBox>();
	lights_[1] = std::make_unique<SpotLightBox>();
}

void TwinSpotLightBox::Initialize()
{
}

void TwinSpotLightBox::Update()
{
	*lights_[1]->spotLights_[0]->light_ = *lights_[0]->spotLights_[0]->light_;
	lights_[1]->spotLights_[0]->light_->direction.x *= -1;
	lights_[1]->box_->transform_ = lights_[0]->box_->transform_;
	lights_[1]->box_->transform_.translate_.x *= -1;

	lights_[0]->Update();
	lights_[1]->Update();
}

void TwinSpotLightBox::Draw(Camera* camera)
{
	lights_[0]->Draw(camera);
	lights_[1]->Draw(camera);
}
