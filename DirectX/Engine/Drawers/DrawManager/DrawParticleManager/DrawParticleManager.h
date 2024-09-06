#pragma once
#include "Matrix4x4.h"
#include "Drawers/IDrawManager/IDrawManager.h"
#include <wrl.h>
#include <vector>
#include <memory>

class Camera;
class ModelData;
enum class BlendMode;
class GPUParticle;

class DrawParticleManager : public IDrawManager
{
public:
	DrawParticleManager();
	void Draw(const GPUParticle& particle,const Camera& camera, const BlendMode& blendMode);

private:
	void SetPerView();
	class PerView
	{
	public:
		PerView();
		~PerView();
		struct PerViewData {
			Matrix4x4 viewProjection;
			Matrix4x4 billboardMatrix;
		};
		Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource;
		PerViewData* perViewData;
	};
	std::vector<std::unique_ptr<PerView>> perViews_;
	const ModelData* modelData_;
	const Camera* camera_ = nullptr;
};