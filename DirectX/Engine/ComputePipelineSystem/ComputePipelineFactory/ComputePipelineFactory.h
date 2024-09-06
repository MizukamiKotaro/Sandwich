#pragma once
#include "ComputePipelineSystem/IComputePipelineFactory/IComputePipelineFactory.h"

class ComputePipelineFactory : public IComputePipelineFactory
{
public:
	ComputePipeline* CreateComputePipeline(ComputePipelineType pipelineType) override;
};
