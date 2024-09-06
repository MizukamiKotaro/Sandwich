#pragma once
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

class SRVDescriptorHeap : public DescriptorHeap
{
public:
	SRVDescriptorHeap();
	static const UINT& GetTextureMaxNum();
};

class RTVDescriptorHeap : public DescriptorHeap
{
public:
	RTVDescriptorHeap();
};

class DSVDescriptorHeap : public DescriptorHeap
{
public:
	DSVDescriptorHeap();
};