#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class SpotLightDepthGraphicsPipline : public GraphicsPipeline
{
public:
	SpotLightDepthGraphicsPipline();
private:
	void InitializePSO() override;
};