#include "ComputePipelineFactory.h"
#include "ComputePipelineSystem/ComputePipelineTypeConfig.h"
#include "ComputePipelineSystem/ComputePipelines/SkinningComputePipeline/SkinningComputePipeline.h"
#include "ComputePipelineSystem/ComputePipelines/ParticleInitializeComputePipeline/ParticleInitializeComputePipeline.h"

ComputePipeline* ComputePipelineFactory::CreateComputePipeline(ComputePipelineType pipelineType)
{
	ComputePipeline* pipeline = nullptr;
	switch (pipelineType)
	{
	case ComputePipelineType::SKINNING:
		pipeline = new SkinningComputePipeline();
		break;
	case ComputePipelineType::PARTICLE_INITIALIZE:
		pipeline = new ParticleInitializeComputePipeline();
		break;
	default:
		break;
	}
	return pipeline;
}
