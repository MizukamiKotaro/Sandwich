#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class BasePostEffectGraphicsPipeline : public GraphicsPipeline
{
public:
	BasePostEffectGraphicsPipeline(const std::string& psFileName);
private:
	BasePostEffectGraphicsPipeline() = default;
	void InitializePSO() override;
	void InitializePSO(const std::string& psFileName);
};