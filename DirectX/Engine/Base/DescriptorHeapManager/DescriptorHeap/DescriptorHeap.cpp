#include "DescriptorHeap.h"
#include <cassert>
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "DescriptorHeapManager/DescriptorHeaps/DescriptorHeaps.h"

DescriptorHeap::~DescriptorHeap()
{
	descriptors_.clear();
	heap_->Release();
}

void DescriptorHeap::DeleteDescriptor(const DescriptorHandles* handles) 
{
	if (descriptors_.size() != 0) {
		descriptors_[handles->no]->isUse = false;
	}
}

const DescriptorHandles* DescriptorHeap::GetNewDescriptorHandles()
{
	if (heapType_ == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) {
		for (UINT i = SRVDescriptorHeap::GetTextureMaxNum(); i < UINT(descriptors_.size()); i++) {
			if (!descriptors_[i]->isUse) {
				descriptors_[i]->isUse = true;
				return &descriptors_[i]->handles;
			}
		}
	}
	else {
		for (std::unique_ptr<Descriptor>& descriptor : descriptors_) {
			if (!descriptor->isUse) {
				descriptor->isUse = true;
				return &descriptor->handles;
			}
		}
	}

	return nullptr;
}

const DescriptorHandles* DescriptorHeap::GetNewTextureDescriptorHandles()
{
	for (UINT i = 0; i < SRVDescriptorHeap::GetTextureMaxNum(); i++) {
		if (!descriptors_[i]->isUse) {
			descriptors_[i]->isUse = true;
			return &descriptors_[i]->handles;
		}
	}

	return nullptr;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DescriptorHeap::GetTextureHandle()
{
	return descriptors_[0]->handles.gpuHandle;
}

ID3D12DescriptorHeap* DescriptorHeap::CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	//ディスクリプタヒープの生成
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	//ディスクリプタヒープが作られなかったので起動しない
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

void DescriptorHeap::Initialize(UINT numDescriptors)
{
	ID3D12Device* device = DirectXBase::GetInstance()->GetDevice();
	descriptors_.clear();
	descriptors_.resize(numDescriptors);
	Descriptor descriptor;
	for (UINT i = 0; i < numDescriptors; i++) {
		descriptor.isUse = false;
		descriptor.handles.no = i;
		descriptor.handles.cpuHandle = heap_->GetCPUDescriptorHandleForHeapStart();
		descriptor.handles.cpuHandle.ptr += (device->GetDescriptorHandleIncrementSize(heapType_) * descriptor.handles.no);

		if (heap_->GetDesc().Flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) {
			descriptor.handles.gpuHandle = heap_->GetGPUDescriptorHandleForHeapStart();
			descriptor.handles.gpuHandle.ptr += (device->GetDescriptorHandleIncrementSize(heapType_) * descriptor.handles.no);
		}

		descriptors_[i] = std::make_unique<Descriptor>(descriptor);
	}
}

uint32_t DescriptorHeap::GetTextureNum(const DescriptorHandles* handle)
{
	for (uint32_t i = 0; i < SRVDescriptorHeap::GetTextureMaxNum(); i++) {
		if (descriptors_[i]->isUse && &descriptors_[i]->handles == handle) {
			return i;
		}
	}
	return 1;
}
