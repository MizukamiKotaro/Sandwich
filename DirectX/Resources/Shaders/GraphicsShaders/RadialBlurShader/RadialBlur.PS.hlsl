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

struct RadialBlurData {
	float32_t2 center;
	float32_t width;
	int32_t numSamples;
};
ConstantBuffer<RadialBlurData> gRadialBlur : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	
	float32_t2 direction = input.texcoord - gRadialBlur.center;
	float32_t3 outputColor = float32_t3(0.0f,0.0f,0.0f);
	for(int32_t i = 0; i < gRadialBlur.numSamples; i++){
		float32_t2 texcoord = input.texcoord + direction * gRadialBlur.width * float32_t(i);
		outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb; 
	}
	outputColor.rgb *= rcp(gRadialBlur.numSamples);

	PixelShaderOutput output;
	output.color.rgb = outputColor;
	output.color.a = 1.0f;
	return output;
}