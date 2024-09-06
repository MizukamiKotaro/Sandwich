#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class OutlineGraphicsPipeline : public GraphicsPipeline
{
public:
	OutlineGraphicsPipeline();
private:
	void InitializePSO() override;
};