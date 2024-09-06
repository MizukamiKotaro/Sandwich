#include "GPUParticle.h"
#include "ParticleManager.h"
#include "ModelDataManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Drawers/GPUParticle/GPUParticleData.h"
#include "DirectXBase/DirectXBase.h"
#include "Base/DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "Base/DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Drawers/DrawManager/DrawManager.h"
#include "ComputePipelineSystem/ComputePipelineManager/ComputePipelineManager.h"
#include "ComputePipelineSystem/ComputePipelineTypeConfig.h"

GPUParticle::GPUParticle(const std::string& particleName, const std::string& textureName, bool isStageEditor)
{
	CreateParticle(particleName, textureName, isStageEditor);

	// uavの作成
	particleResouce_ = DirectXBase::CreateBufferResource(sizeof(GPUParticleData) * 1024, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	particleUAVHandle_ = DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->GetNewDescriptorHandles();
	
	D3D12_UNORDERED_ACCESS_VIEW_DESC outputVertexUAVDesc{};
	outputVertexUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	outputVertexUAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	outputVertexUAVDesc.Buffer.FirstElement = 0;
	outputVertexUAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	outputVertexUAVDesc.Buffer.NumElements = UINT(1024);
	outputVertexUAVDesc.Buffer.StructureByteStride = sizeof(GPUParticleData);
	DirectXBase::GetInstance()->GetDevice()->CreateUnorderedAccessView(particleResouce_.Get(), nullptr, &outputVertexUAVDesc, particleUAVHandle_->cpuHandle);

	particleSRVHandle_ = DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->GetNewDescriptorHandles();

	D3D12_SHADER_RESOURCE_VIEW_DESC influenceSRVDesc{};
	influenceSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	influenceSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	influenceSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	influenceSRVDesc.Buffer.FirstElement = 0;
	influenceSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	influenceSRVDesc.Buffer.NumElements = UINT(1024);
	influenceSRVDesc.Buffer.StructureByteStride = sizeof(GPUParticleData);
	DirectXBase::GetInstance()->GetDevice()->CreateShaderResourceView(particleResouce_.Get(), &influenceSRVDesc, particleSRVHandle_->cpuHandle);


}

GPUParticle::~GPUParticle()
{
	DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->DeleteDescriptor(particleUAVHandle_);
	DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->DeleteDescriptor(particleSRVHandle_);
	particleResouce_->Release();
}

void GPUParticle::Initialize()
{
}

void GPUParticle::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	ID3D12DescriptorHeap* descriptorHeaps[] = { DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->GetHeap() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);
	ComputePipelineManager::GetInstance()->PreCompute(ComputePipelineType::PARTICLE_INITIALIZE);
	commandList_->SetComputeRootDescriptorTable(0, particleUAVHandle_->gpuHandle);
	commandList_->Dispatch(1024, 1, 1);

	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrierDesc.Transition.pResource = particleResouce_.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS; 
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE; 
	barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList_->ResourceBarrier(1, &barrierDesc);
	if(deltaTime){}
}

void GPUParticle::Draw(const Camera& camera, const BlendMode& blend)
{
	drawManager_->Draw(*this, camera, blend);
}

const D3D12_GPU_DESCRIPTOR_HANDLE& GPUParticle::GetParticleGPUDescriptorHandle() const
{
	return particleSRVHandle_->gpuHandle;
}

void GPUParticle::SetGlobalVariable()
{
	if (globalVariable_) {

	}
	if (stageEditor_) {

	}

	ApplyGlobalVariable();
}

void GPUParticle::ApplyGlobalVariable()
{
	if (globalVariable_) {

	}
	if (stageEditor_) {

	}
}
