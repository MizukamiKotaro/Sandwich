#pragma once
class ComputePipeline;
enum class ComputePipelineType;

class IComputePipelineFactory
{
public:
	virtual ~IComputePipelineFactory() = default;

	virtual ComputePipeline* CreateComputePipeline(ComputePipelineType pipelineType) = 0;
};