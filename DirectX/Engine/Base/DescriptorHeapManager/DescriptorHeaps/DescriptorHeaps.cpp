#include "DescriptorHeaps.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"

const size_t kNumDescriptors_ = 256;
const size_t kNumSRVDescriptors_ = 2048;
const UINT kNumTextures_ = 1024;

SRVDescriptorHeap::SRVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumSRVDescriptors_, true);
	Initialize(kNumSRVDescriptors_);
}

const UINT& SRVDescriptorHeap::GetTextureMaxNum()
{
	return kNumTextures_;
}

RTVDescriptorHeap::RTVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
	Initialize(kNumDescriptors_);
}

DSVDescriptorHeap::DSVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
	Initialize(kNumDescriptors_);
}