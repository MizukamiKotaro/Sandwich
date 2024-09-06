#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct HighLumiData {
	float min;
	float max;
	int32_t isToWhite;
};
ConstantBuffer<HighLumiData> gHighLumi : register(b1);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	float grayScale = dot(textureColor.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));
	float extract = smoothstep(gHighLumi.min, gHighLumi.max, grayScale);

	output.color = textureColor * extract;

	if (gHighLumi.isToWhite == 1) {
		if (output.color.r != 0.0f && output.color.g != 0.0f && output.color.b != 0.0f && output.color.a != 0.0f) {
			output.color.r = 1.0f;
			output.color.g = 1.0f;
			output.color.b = 1.0f;
			output.color.a = 1.0f;
		}
	}

	return output;
}