#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class PointLightGraphicsPipeline : public GraphicsPipeline
{
public:
	PointLightGraphicsPipeline();
private:
	void InitializePSO() override;
};