#include "Outline.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "Externals/DirectXTex/d3dx12.h"

Outline::Outline()
{
	piplineType_ = PipelineType::OUTLINE;
	outlineData_ = nullptr;
	CreatePostEffect();
}

Outline::~Outline()
{
	outlineResource_->Release();
	srvHeap_->DeleteDescriptor(depthHandles_);
}

void Outline::Draw(BlendMode blendMode)
{
	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	ToReadBarrier();

	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	commandList->SetGraphicsRootConstantBufferView(2, outlineResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(3, depthHandles_->gpuHandle);
	commandList->DrawInstanced(3, 1, 0, 0);

	ToWriteBarrier();
}

void Outline::Draw(const Camera& camera, BlendMode blendMode)
{
	materialData_->color = color_;
	outlineData_->projectionInverse = camera.GetInverseProjection();

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	ToReadBarrier();

	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	commandList->SetGraphicsRootConstantBufferView(2, outlineResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(3, depthHandles_->gpuHandle);
	commandList->DrawInstanced(3, 1, 0, 0);

	ToWriteBarrier();
}

void Outline::ToReadBarrier()
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsvResource_.Get(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	commandList_->ResourceBarrier(1, &barrier);
}

void Outline::ToWriteBarrier()
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	commandList_->ResourceBarrier(1, &barrier);
}

void Outline::CreateOutlineRes()
{
	outlineResource_ = DirectXBase::CreateBufferResource(sizeof(OutlineData));
	outlineResource_->Map(0, nullptr, reinterpret_cast<void**>(&outlineData_));
	outlineData_->projectionInverse = Matrix4x4::MakeIdentity4x4();
	outlineData_->lengthChange = 75.0f;
	outlineData_->maxWidth = 7;
	outlineData_->maxLength = 1000.0f;
}

void Outline::CreateDepthRes()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	depthHandles_ = srvHeap_->GetNewDescriptorHandles();

	device_->CreateShaderResourceView(dsvResource_.Get(), &srvDesc, depthHandles_->cpuHandle);
}

void Outline::CreateResources()
{
	BasePostEffect::CreateResources();
	CreateOutlineRes();
	CreateDepthRes();
}
