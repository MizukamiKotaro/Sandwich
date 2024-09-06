#include "PointLight.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "Drawers/DrawManager/DrawManager.h"

PointLight::PointLight()
{
	lightType_ = LightType::kPointLight;

	resource_ = DirectXBase::CreateBufferResource(sizeof(PointLightData));
	light_ = nullptr;
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 0.0f;
	light_->radius = 1.0f;
	light_->decay = 0.1f;

	isDraw_ = true;
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	if (light_->radius <= 0.0f) {
		light_->radius = 0.01f;
	}
	if (light_->decay <= 0.0f) {
		light_->decay = 0.01f;
	}
}

void PointLight::Draw(const Camera& camera, const BlendMode& blendMode) const
{
	if (isDraw_) {
		drawManager_->Draw(*this, camera, blendMode);
	}
}
