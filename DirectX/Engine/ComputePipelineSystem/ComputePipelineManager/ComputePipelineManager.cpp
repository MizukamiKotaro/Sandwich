#include "ComputePipelineManager.h"
#include "ComputePipelineSystem/ComputePipelineTypeConfig.h"
#include "ComputePipelineSystem/ComputePipelineFactory/ComputePipelineFactory.h"
#include "ComputePipelineSystem/ComputePipeline/ComputePipeline.h"

ComputePipelineManager* ComputePipelineManager::GetInstance()
{
	static ComputePipelineManager instance;
	return &instance;
}

void ComputePipelineManager::Initialize()
{
	pipelineFactory_ = std::make_unique<ComputePipelineFactory>();
	currentPiplineType_ = ComputePipelineType::SKINNING;

	pipelineMap_[currentPiplineType_].reset(pipelineFactory_->CreateComputePipeline(currentPiplineType_));
	pipelineMap_[currentPiplineType_]->PreCompute();
}

void ComputePipelineManager::PreCompute(ComputePipelineType type)
{
	if (currentPiplineType_ != type) {
		currentPiplineType_ = type;

		if (pipelineMap_.find(type) == pipelineMap_.end()) {
			pipelineMap_[currentPiplineType_].reset(pipelineFactory_->CreateComputePipeline(currentPiplineType_));
		}
	}
	pipelineMap_[type]->PreCompute();
}
