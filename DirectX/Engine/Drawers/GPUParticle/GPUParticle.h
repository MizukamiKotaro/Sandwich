#pragma once
#include "IParticle.cpp/IParticle.h"
#include <wrl.h>
#include <d3d12.h>


class DescriptorHandles;

class GPUParticle : public IParticle
{
public:
	GPUParticle(const std::string& particleName, const std::string& textureName, bool isStageEditor = false);
	~GPUParticle();

	void Initialize() override;

	void Update(const float& deltaTime) override;

	void Draw(const Camera& camera, const BlendMode& blend = BlendMode::kBlendModeNormal) override;

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetParticleGPUDescriptorHandle() const;

private:
	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

	const DescriptorHandles* particleUAVHandle_;
	const DescriptorHandles* particleSRVHandle_;
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResouce_;

};