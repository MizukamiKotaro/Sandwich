#pragma once

#include <wrl.h>
#include <string>
#include <vector>
#include <memory>
#include "DescriptorHeapManager/Descriptor/Descriptor.h"

class DescriptorHeap {
public:
	
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~DescriptorHeap();
public:
	void DeleteDescriptor(const DescriptorHandles* handles);
	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }
	const DescriptorHandles* GetNewDescriptorHandles();
	const DescriptorHandles* GetNewTextureDescriptorHandles();
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetTextureHandle();
	static ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	void Initialize(UINT numDescriptors);

	uint32_t GetTextureNum(const DescriptorHandles* handle);

protected:

	ComPtr<ID3D12DescriptorHeap> heap_;
	D3D12_DESCRIPTOR_HEAP_TYPE heapType_;
	std::vector<std::unique_ptr<Descriptor>> descriptors_;
};