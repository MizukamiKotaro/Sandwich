#pragma once
#include "ComputePipelineSystem/ComputePipeline/ComputePipeline.h"

class SkinningComputePipeline : public ComputePipeline
{
public:
	SkinningComputePipeline();
private:
	void InitializePSO() override;
};