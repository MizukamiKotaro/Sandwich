#include "DrawManager.h"
#include "Camera.h"
#include "Model.h"
#include "SkinningModel/SkinningModel.h"
#include "RigidAnimationModel/RigidAnimationModel.h"
#include "Drawers/GPUParticle/GPUParticle.h"
#include "DrawModelManager/DrawModelManager.h"
#include "DrawLightManager/DrawLightManager.h"
#include "DrawParticleManager/DrawParticleManager.h"

DrawManager* DrawManager::GetInstance()
{
	static DrawManager instance;
	return &instance;
}

void DrawManager::Initialize()
{
	drawModelManager_ = std::make_unique<DrawModelManager>();
	drawLightManager_ = std::make_unique<DrawLightManager>();
	drawParticleManager_ = std::make_unique<DrawParticleManager>();
}

void DrawManager::Reset()
{
	drawModelManager_->Reset();
	drawLightManager_->Reset();
	drawParticleManager_->Reset();
}

void DrawManager::Draw(const Model& model, const Camera& camera, const BlendMode& blendMode)
{
	drawModelManager_->Draw(model, camera, blendMode);
}

void DrawManager::Draw(const RigidAnimationModel& model, const Camera& camera, const BlendMode& blendMode)
{
	drawModelManager_->Draw(model, camera, blendMode);
}

void DrawManager::Draw(const SkinningModel& model, const Camera& camera, const BlendMode& blendMode)
{
	drawModelManager_->Draw(model, camera, blendMode);
}

void DrawManager::Draw(const PointLight& light, const Camera& camera, const BlendMode& blendMode)
{
	drawLightManager_->Draw(light, camera, blendMode);
}

void DrawManager::Draw(const SpotLight& light, const Camera& camera, const BlendMode& blendMode)
{
	drawLightManager_->Draw(light, camera, blendMode);
}

void DrawManager::Draw(const GPUParticle& particle, const Camera& camera, const BlendMode& blendMode)
{
	drawParticleManager_->Draw(particle, camera, blendMode);
}
