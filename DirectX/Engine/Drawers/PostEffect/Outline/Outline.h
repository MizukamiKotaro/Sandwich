#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Camera;

class Outline : public BasePostEffect
{
public:
	Outline();
	~Outline() override;

	/// <summary>
	/// 描画処理
	/// </summary>
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
private:
	void CreateOutlineRes();
	void CreateDepthRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> outlineResource_;
	const DescriptorHandles* depthHandles_;
public:
	OutlineData* outlineData_;
};