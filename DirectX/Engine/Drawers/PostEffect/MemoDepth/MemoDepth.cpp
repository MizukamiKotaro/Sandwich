#include "MemoDepth.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "Externals/DirectXTex/d3dx12.h"

MemoDepth::MemoDepth()
{
	piplineType_ = PipelineType::COPY;
	CreatePostEffect();
}

MemoDepth::~MemoDepth()
{
	srvHeap_->DeleteDescriptor(depthHandles_);
}

void MemoDepth::ToReadBarrier()
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsvResource_.Get(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	commandList_->ResourceBarrier(1, &barrier);
}

void MemoDepth::ToWriteBarrier()
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsvResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	commandList_->ResourceBarrier(1, &barrier);
}

void MemoDepth::CreateDepthRes()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	depthHandles_ = srvHeap_->GetNewDescriptorHandles();

	device_->CreateShaderResourceView(dsvResource_.Get(), &srvDesc, depthHandles_->cpuHandle);
}

void MemoDepth::CreateResources()
{
	BasePostEffect::CreateResources();
	CreateDepthRes();
}
