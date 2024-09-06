#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class GrayscaleGraphicsPipeline : public GraphicsPipeline
{
public:
	GrayscaleGraphicsPipeline();
private:
	void InitializePSO() override;
};