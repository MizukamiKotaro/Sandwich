#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class SkyboxGraphicsPipeline : public GraphicsPipeline
{
public:
	SkyboxGraphicsPipeline();
private:
	void InitializePSO() override;
};