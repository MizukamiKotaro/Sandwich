#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Camera;

class MemoDepth : public BasePostEffect
{
public:
	MemoDepth();
	~MemoDepth() override;

	const DescriptorHandles& GetDepthHandle() { return *depthHandles_; }
	void ToReadBarrier();
	void ToWriteBarrier();
private:
	void CreateDepthRes();
	void CreateResources() override;
private:
	const DescriptorHandles* depthHandles_;
};