#include "GaussianBlur.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Gaussian::Gaussian(const bool& isRender, const bool& isDepth, const Vector2& size)
{
	piplineType_ = PipelineType::GAUSSIAN_BLUR;

	gaussianBlurData_ = nullptr;

	CreatePostEffect(isRender, isDepth, size);
}

Gaussian::~Gaussian()
{
	gaussianBlurResource_->Release();
}

void Gaussian::Draw(BlendMode blendMode)
{
	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(2, gaussianBlurResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(3, 1, 0, 0);

}

void Gaussian::CreateGaussianBlurRes()
{
	gaussianBlurResource_ = DirectXBase::CreateBufferResource(sizeof(GaussianBlurData));

	gaussianBlurResource_->Map(0, nullptr, reinterpret_cast<void**>(&gaussianBlurData_));

	gaussianBlurData_->width = 9;
	gaussianBlurData_->height = 0;
	gaussianBlurData_->sigma = 2.0f;
}

void Gaussian::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateGaussianBlurRes();
}

GaussianBlur::GaussianBlur(const bool& isRender, const bool& isDepth, const Vector2& size)
{
	if (size.x == 0.0f || size.y == 0.0f) {
		gaussian0_ = std::make_unique<Gaussian>(isRender, isDepth);
		gaussian1_ = std::make_unique<Gaussian>(isRender, isDepth);
	}
	else {
		gaussian0_ = std::make_unique<Gaussian>(isRender, isDepth, size);
		gaussian1_ = std::make_unique<Gaussian>(isRender, isDepth, size);
	}
	gaussian1_->gaussianBlurData_->height = 1;
}

void GaussianBlur::Initialize()
{
}

void GaussianBlur::Draw(BlendMode blendMode)
{
	gaussian1_->Draw(blendMode);
}

void GaussianBlur::PreDrawScene()
{
	gaussian0_->PreDrawScene();
}

void GaussianBlur::PostDrawScene()
{
	gaussian0_->PostDrawScene();
	gaussian1_->PreDrawScene();
	gaussian0_->Draw();
	gaussian1_->PostDrawScene();
}

void GaussianBlur::SetSize(const int32_t& size)
{
	gaussian0_->gaussianBlurData_->width = size;
	gaussian1_->gaussianBlurData_->width = size;
}

void GaussianBlur::SetSigma(const float& sigma)
{
	gaussian0_->gaussianBlurData_->sigma = sigma;
	gaussian1_->gaussianBlurData_->sigma = sigma;
}
