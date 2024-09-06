#include "GraphicsPipelineFactory.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ModelGraphicsPipline/ModelGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ParticleGraphicsPipeline/ParticleGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SpriteGraphicsPipeline/SpriteGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/PointLightGraphicsPipline/PointLightGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SpotLightGraphicsPipline/SpotLightGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/InstancingModelGraphicsPipline/InstancingModelGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/CopyGraphicsPipeline/CopyGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/GrayscaleGraphicsPipeline/GrayscaleGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/VignetteGraphicsPipeline/VignetteGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SkyboxGraphicsPipeline/SkyboxGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/OutlineGraphicsPipeline/OutlineGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/BasePostEffectGraphicsPipeline/BasePostEffectGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/GPUParticleGraphicsPipeline/GPUParticleGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/LightDepthPipeline/LightDepthPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SpotLightDepthGraphicsPipline/SpotLightDepthGraphicsPipline.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

GraphicsPipeline* GraphicsPipelineFactory::CreateGraphicsPipeline(PipelineType pipelineType)
{
	GraphicsPipeline* graphicsPipeline = nullptr;

	switch (pipelineType)
	{
	case PipelineType::SPRITE:
		graphicsPipeline = new SpriteGraphicsPipeline();
		break;
	case PipelineType::COPY:
		graphicsPipeline = new CopyGraphicsPipeline();
		break;
	case PipelineType::MODEL:
		graphicsPipeline = new ModelGraphicsPipeline();
		break;
	case PipelineType::PARTICLE:
		graphicsPipeline = new ParticleGraphicsPipeline();
		break;
	case PipelineType::POINT_LIGHT:
		graphicsPipeline = new PointLightGraphicsPipeline();
		break;
	case PipelineType::SPOT_LIGHT:
		graphicsPipeline = new SpotLightGraphicsPipeline();
		break;
	case PipelineType::CONTRAST:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("ContrastShader/Contrast.PS.hlsl");
		break;
	case PipelineType::HIGH_LUMI:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("HighLumiShader/HighLumi.PS.hlsl");
		break;
	case PipelineType::BLUR:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("BlurShader/Blur.PS.hlsl");
		break;
	case PipelineType::GAUSSIAN_BLUR:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("GaussianBlurShader/GaussianBlur.PS.hlsl");
		break;
	case PipelineType::SCAN_NOISE:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("ScanNoiseShader/ScanNoise.PS.hlsl");
		break;
	case PipelineType::MOSAIC:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("MosaicShader/Mosaic.PS.hlsl");
		break;
	case PipelineType::NEGA_POSI_INVERSE:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("NegaPosiInverseShader/NegaPosiInverse.PS.hlsl");
		break;
	case PipelineType::RGB_SHIFT:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("RGBShiftShader/RGBShift.PS.hlsl");
		break;
	case PipelineType::INSTANCING_MODEL:
		graphicsPipeline = new InstancingModelGraphicsPipeline();
		break;
	case PipelineType::NOISE:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("NoiseShader/Noise.PS.hlsl");
		break;
	case PipelineType::WATER_OUTLINE:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("WaterOutlineShader/WaterOutline.PS.hlsl");
		break;
	case PipelineType::GRAYSCALE:
		graphicsPipeline = new GrayscaleGraphicsPipeline();
		break;
	case PipelineType::VIGNETTE:
		graphicsPipeline = new VignetteGraphicsPipeline();
		break;
	case PipelineType::SMOOTHING:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("SmoothingShader/Smoothing.PS.hlsl");
		break;
	case PipelineType::SKYBOX:
		graphicsPipeline = new SkyboxGraphicsPipeline();
		break;
	case PipelineType::OUTLINE:
		graphicsPipeline = new OutlineGraphicsPipeline();
		break;
	case PipelineType::RADIAL_BLUR:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("RadialBlurShader/RadialBlur.PS.hlsl");
		break;
	case PipelineType::HSV_FILTER:
		graphicsPipeline = new BasePostEffectGraphicsPipeline("HSVFilterShader/HSVFilter.PS.hlsl");
		break;
	case PipelineType::GPU_PARTICLE:
		graphicsPipeline = new GPUParticleGraphicsPipeline();
		break;
	case PipelineType::LIGHT_DEPTH:
		graphicsPipeline = new LightDepthPipeline();
		break;
	case PipelineType::SPOT_LIGHT_DEPTH:
		graphicsPipeline = new SpotLightDepthGraphicsPipline();
		break;
	default:
		break;
	}
	
	return graphicsPipeline;
}