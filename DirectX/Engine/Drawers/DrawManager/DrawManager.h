#pragma once
#include <memory>

enum class BlendMode;
class Camera;
class Model;
class RigidAnimationModel;
class SkinningModel;
class PointLight;
class SpotLight;
class GPUParticle;
class DrawModelManager;
class DrawLightManager;
class DrawParticleManager;

class DrawManager
{
public:
	static DrawManager* GetInstance();

	void Initialize();
	void Reset();

	void Draw(const Model& model,const Camera& camera, const BlendMode& blendMode);
	void Draw(const RigidAnimationModel& model, const Camera& camera, const BlendMode& blendMode);
	void Draw(const SkinningModel& model, const Camera& camera, const BlendMode& blendMode);
	void Draw(const PointLight& light, const Camera& camera, const BlendMode& blendMode);
	void Draw(const SpotLight& light, const Camera& camera, const BlendMode& blendMode);
	void Draw(const GPUParticle& particle, const Camera& camera, const BlendMode& blendMode);
private:
	DrawManager() = default;
	~DrawManager() = default;
	DrawManager(const DrawManager&) = delete;
	const DrawManager& operator=(const DrawManager&) = delete;

	std::unique_ptr<DrawModelManager> drawModelManager_;
	std::unique_ptr<DrawLightManager> drawLightManager_;
	std::unique_ptr<DrawParticleManager> drawParticleManager_;
};