#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class GPUParticleGraphicsPipeline : public GraphicsPipeline
{
public:
	GPUParticleGraphicsPipeline();
private:
	void InitializePSO() override;
};