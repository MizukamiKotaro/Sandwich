#include "Vignette.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

Vignette::Vignette()
{
	piplineType_ = PipelineType::VIGNETTE;

	CreatePostEffect();
}