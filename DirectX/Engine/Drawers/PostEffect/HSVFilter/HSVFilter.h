#pragma once
#include "BasePostEffect/BasePostEffect.h"

class HSVFilter : public BasePostEffect
{
public:
	HSVFilter();
	~HSVFilter() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct HSVData {
		float hue; // 色相
		float saturate; // 彩度
		float value; // 明度
	};
private:
	void CreateHSVRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> hsvResource_;
public:
	HSVData* hsvData_;
};