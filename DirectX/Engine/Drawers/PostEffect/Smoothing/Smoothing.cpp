#include "Smoothing.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Smoothing::Smoothing()
{
	piplineType_ = PipelineType::SMOOTHING;
	smoothingData_ = nullptr;
	CreatePostEffect();
}

Smoothing::~Smoothing()
{
	smoothingResource_->Release();
}

void Smoothing::Draw(BlendMode blendMode)
{
	materialData_->color = color_;
	PreDraw();
	psoManager_->SetBlendMode(piplineType_, blendMode);
	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	commandList->SetGraphicsRootConstantBufferView(2, smoothingResource_->GetGPUVirtualAddress());
	commandList->DrawInstanced(3, 1, 0, 0);
}

void Smoothing::CreateSmoothingRes()
{
	smoothingResource_ = DirectXBase::CreateBufferResource(sizeof(SmoothingData));
	smoothingResource_->Map(0, nullptr, reinterpret_cast<void**>(&smoothingData_));
	smoothingData_->height = 9;
	smoothingData_->width = 9;
}

void Smoothing::CreateResources()
{
	BasePostEffect::CreateResources();
	CreateSmoothingRes();
}
