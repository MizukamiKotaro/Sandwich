#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class SpotLightGraphicsPipeline : public GraphicsPipeline
{
public:
	SpotLightGraphicsPipeline();
private:
	void InitializePSO() override;
};