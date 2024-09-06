#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class LightDepthPipeline : public GraphicsPipeline
{
public:
	LightDepthPipeline();
private:
	void InitializePSO() override;
};