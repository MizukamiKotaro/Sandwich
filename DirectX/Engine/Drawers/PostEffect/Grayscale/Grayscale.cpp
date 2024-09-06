#include "Grayscale.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

Grayscale::Grayscale()
{
	piplineType_ = PipelineType::GRAYSCALE;

	CreatePostEffect();
}