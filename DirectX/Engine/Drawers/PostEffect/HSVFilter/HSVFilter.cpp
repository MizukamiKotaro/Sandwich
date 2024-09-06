#include "HSVFilter.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

HSVFilter::HSVFilter()
{
	piplineType_ = PipelineType::HSV_FILTER;

	hsvData_ = nullptr;

	CreatePostEffect();
}

HSVFilter::~HSVFilter()
{
	hsvResource_->Release();
}

void HSVFilter::Draw(BlendMode blendMode)
{

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(2, hsvResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(3, 1, 0, 0);

}

void HSVFilter::CreateHSVRes()
{
	hsvResource_ = DirectXBase::CreateBufferResource(sizeof(HSVData));
	hsvResource_->Map(0, nullptr, reinterpret_cast<void**>(&hsvData_));
	hsvData_->hue = 0.0f;
	hsvData_->saturate = 0.0f;
	hsvData_->value = 0.0f;
}

void HSVFilter::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateHSVRes();
}

