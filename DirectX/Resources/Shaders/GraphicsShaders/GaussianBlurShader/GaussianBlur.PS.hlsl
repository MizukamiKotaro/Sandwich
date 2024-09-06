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

struct GaussianBlurData {
	int32_t width;
	int32_t height;
	float sigma;
};
ConstantBuffer<GaussianBlurData> gGaussianBlur : register(b1);

static const float PI = 3.14159265f;

float gauss(float x, float sigma){
	return rcp(sqrt(2.0f * PI) * sigma) * exp(-(x * x) * rcp(2.0f * sigma * sigma));
}

float32_t4 GaussianBlur(float32_t2 uv, float sigma, float32_t2 dir, int32_t size, float32_t2 uvStepSize){
	float32_t3 o = 0;
	float sum = 0;
	float32_t2 uvOffset;
	float weight;

	for(int32_t x = -size >> 1; x <= size >> 1; x++){
		uvOffset = uv + x * uvStepSize * dir;
		weight = gauss(x, sigma);
		o += gTexture.Sample(gSampler, uvOffset).rgb * weight;
		sum += weight;
	}
	o *= rcp(sum);
	return float32_t4(o.r,o.g,o.b,1.0f);
}

float32_t4 GaussianBlurLinear(float32_t2 uv, float sigma, float32_t2 dir, int32_t size, float32_t2 uvStepSize){
	float32_t3 o = 0;
	float sum = 0;
	float32_t2 uvOffset;
	float weight;

	for(int x = -size >> 1; x <= size >> 1; x += 2){
		uvOffset = uv + (x + 0.5f) * uvStepSize * dir;
		weight = gauss(x, sigma) + gauss(x + 1, sigma);
		o += gTexture.Sample(gSampler, uvOffset).rgb * weight;
		sum += weight;
	}
	o *= rcp(sum);
	return float32_t4(o.r,o.g,o.b,1.0f);
}

PixelShaderOutput main(VertexShaderOutput input) {
	uint32_t width, height;
	gTexture.GetDimensions(width,height);
	float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
	float32_t2 dir;
	if(gGaussianBlur.height == 0){
		dir = float32_t2(1.0f,0.0f);
	}
	else {
		dir = float32_t2(0.0f,1.0f);
	}

	PixelShaderOutput output;

	output.color = GaussianBlurLinear(input.texcoord, gGaussianBlur.sigma, dir, gGaussianBlur.width, uvStepSize);

	return output;
}