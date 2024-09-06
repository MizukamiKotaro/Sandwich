#pragma once
#include "ComputePipelineSystem/ComputePipeline/ComputePipeline.h"

class ParticleInitializeComputePipeline : public ComputePipeline
{
public:
	ParticleInitializeComputePipeline();
private:
	void InitializePSO() override;
};