#pragma once
#include "BasePostEffect/BasePostEffect.h"
#include "memory"

class Gaussian : public BasePostEffect
{
public:
	Gaussian(const bool& isRender = true, const bool& isDepth = true, const Vector2& size = windowSize_);
	~Gaussian() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct GaussianBlurData {
		int32_t width;
		int32_t height;
		float sigma;
	};
private:
	void CreateGaussianBlurRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> gaussianBlurResource_;
public:
	GaussianBlurData* gaussianBlurData_;
};

class GaussianBlur
{
public:
	GaussianBlur(const bool& isRender = true, const bool& isDepth = true, const Vector2& size = {});
	void Initialize();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);
	/// <summary>
	/// 描画前の処理,ポストエフェクトさせるものの描画処理の前に書く,更新処理の部分に書くのが吉
	/// </summary>
	void PreDrawScene();
	/// <summary>
	/// 描画後の処理,ポストエフェクトさせるものの描画処理の後に書く,更新処理の部分に書くのが吉
	/// </summary>
	void PostDrawScene();

	void SetSize(const int32_t& size);
	const int32_t& GetSize() const { return gaussian0_->gaussianBlurData_->width; }
	void SetSigma(const float& sigma);
	const float& GetSigma() const { return gaussian0_->gaussianBlurData_->sigma; }

private:
	std::unique_ptr<Gaussian> gaussian0_;
	std::unique_ptr<Gaussian> gaussian1_;
};
