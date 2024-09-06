#include "SpotLight.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "Drawers/DrawManager/DrawManager.h"
#include <algorithm>

SpotLight::SpotLight()
{
	lightType_ = LightType::kSpotLight;

	resource_ = DirectXBase::CreateBufferResource(sizeof(SpotLightData));
	light_ = nullptr;
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 0.0f;
	light_->direction = { 0.0f,-1.0f,0.0f };
	light_->distance = 5.0f;
	light_->decay = 0.1f;
	light_->cosAngle = 0.7f;
	light_->cosFalloffStart = 0.8f;

	isDraw_ = true;
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update()
{
	light_->direction = light_->direction.Normalize();

	light_->cosAngle = std::clamp<float>(light_->cosAngle, -1.0f, 0.998f);
	light_->cosFalloffStart = std::clamp<float>(light_->cosFalloffStart, light_->cosAngle + 0.001f, 0.999f);

	if (light_->distance <= 0.0f) {
		light_->distance = 0.01f;
	}
}

void SpotLight::Draw(const Camera& camera, const BlendMode& blendMode) const
{
	if (isDraw_) {
		drawManager_->Draw(*this, camera, blendMode);
	}
}