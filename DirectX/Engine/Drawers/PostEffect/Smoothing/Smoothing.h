#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Smoothing : public BasePostEffect
{
public:
	Smoothing();
	~Smoothing() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct SmoothingData {
		int32_t width;
		int32_t height;
	};
private:
	void CreateSmoothingRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> smoothingResource_;
public:
	SmoothingData* smoothingData_;
};