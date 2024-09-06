#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class ModelGraphicsPipeline : public GraphicsPipeline
{
public:
	ModelGraphicsPipeline();
private:
	void InitializePSO() override;
};