#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct SmoothingData {
	int32_t width;
	int32_t height;
};
ConstantBuffer<SmoothingData> gSmoothing : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	uint32_t width, height;
	gTexture.GetDimensions(width,height);
	float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
	float32_t2 uvIndex = {gSmoothing.width >> 1, gSmoothing.height >> 1};

	PixelShaderOutput output;
	output.color = float32_t4(0.0f,0.0f,0.0f,1.0f);

	for(int32_t x = 0; x < gSmoothing.width; x++){
		for(int32_t y = 0; y < gSmoothing.height; y++){
			float32_t2 texcoord = input.texcoord + float32_t2(x - uvIndex.x, y - uvIndex.y) * uvStepSize; 
			float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			output.color.rgb += fetchColor;
		}
	}
	output.color.rgb *= rcp(gSmoothing.width * gSmoothing.height);

	return output;
}