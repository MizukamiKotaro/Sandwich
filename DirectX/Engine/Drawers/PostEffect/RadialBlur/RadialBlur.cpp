#include "RadialBlur.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

RadialBlur::RadialBlur()
{
	piplineType_ = PipelineType::RADIAL_BLUR;
	radialBlurData_ = nullptr;
	CreatePostEffect();
}

RadialBlur::~RadialBlur()
{
	radialBlurResource_->Release();
}

void RadialBlur::Draw(BlendMode blendMode)
{
	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(2, radialBlurResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(3, 1, 0, 0);

}

void RadialBlur::CreateRadialBlurRes()
{
	radialBlurResource_ = DirectXBase::CreateBufferResource(sizeof(RadialBlurData));
	radialBlurResource_->Map(0, nullptr, reinterpret_cast<void**>(&radialBlurData_));
	radialBlurData_->center = { 0.5f,0.5f };
	radialBlurData_->width = -0.05f;
	radialBlurData_->numSamples = 10;
}

void RadialBlur::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateRadialBlurRes();
}
