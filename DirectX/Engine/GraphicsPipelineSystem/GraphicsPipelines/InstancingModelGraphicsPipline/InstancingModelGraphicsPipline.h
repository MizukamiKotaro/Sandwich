#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class InstancingModelGraphicsPipeline : public GraphicsPipeline
{
public:
	InstancingModelGraphicsPipeline();
private:
	void InitializePSO() override;
};