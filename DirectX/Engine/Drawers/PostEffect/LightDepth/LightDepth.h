#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Camera;

class LightDepth : public BasePostEffect
{
public:
	LightDepth();
	~LightDepth() override;

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);
	struct OutlineData {
		Matrix4x4 projectionInverse;
		float lengthChange;
		float maxLength;
		int32_t maxWidth;
	};

	const DescriptorHandles& GetDepthHandle() { return *depthHandles_; }
	void ToReadBarrier();
	void ToWriteBarrier();

	void SetOutlineTex(const D3D12_GPU_DESCRIPTOR_HANDLE& handle) { outlineTex_ = handle; }
	void SetOutlineDepth(const DescriptorHandles& handle) { outlineDepth_ = &handle; }
	void SetObjectsTex(const D3D12_GPU_DESCRIPTOR_HANDLE& handle) { objectTex_ = handle; }
	void SetObjectsDepth(const DescriptorHandles& handle) { objectDepth_ = &handle; }
private:
	void CreateOutlineRes();
	void CreateDepthRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> outlineResource_;
	const DescriptorHandles* depthHandles_;

	const DescriptorHandles* outlineDepth_;
	D3D12_GPU_DESCRIPTOR_HANDLE outlineTex_;
	const DescriptorHandles* objectDepth_;
	D3D12_GPU_DESCRIPTOR_HANDLE objectTex_;
public:
	OutlineData* outlineData_;

};