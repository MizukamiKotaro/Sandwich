#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D<float32_t> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);


struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct OutlineData {
	float32_t4x4  projectionInverse;
	float32_t lengthChange;
	float32_t maxLength;
	int32_t maxWidth;
};
ConstantBuffer<OutlineData> gOutline : register(b1);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, input.texcoord);
	float32_t4 viewSpace = mul(float32_t4(0.0f,0.0f,ndcDepth,1.0f),gOutline.projectionInverse);
	float32_t viewZ = viewSpace.z * rcp(viewSpace.w);
	float32_t baseZ = viewZ;

	uint32_t width, height;
	gTexture.GetDimensions(width,height);
	float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
	float32_t2 difference = 0;

	if(viewZ <= gOutline.lengthChange){
		for(int32_t x = -1; x <= 1; x++){
			for(int32_t y = -1; y <= 1; y++){
				if(x == 0 && y == 0){
					continue;
				}
				float32_t2 texcoord = input.texcoord + float32_t2(x,y) * uvStepSize;

				ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
				viewSpace = mul(float32_t4(0.0f,0.0f,ndcDepth,1.0f),gOutline.projectionInverse);
				viewZ = viewSpace.z * rcp(viewSpace.w);
				difference.x += viewZ * x * rcp(6);
				difference.y += viewZ * y * rcp(6);
			}
		}
	}
	else{
		viewZ -= gOutline.lengthChange;
		viewZ = saturate(viewZ * rcp(gOutline.maxLength));
		float32_t fWidth = floor((1.0 - viewZ) * 3 + viewZ * gOutline.maxWidth);
		int32_t i = int32_t(fWidth);
		int32_t halfi = i >> 1;
		float32_t k = rcp(i * (i - 1));

		for(int32_t x = -halfi; x <= halfi; x++){
			for(int32_t y = -halfi; y <= halfi; y++){
				if(x == 0 && y == 0){
					continue;
				}
				float32_t2 texcoord = input.texcoord + float32_t2(x,y) * uvStepSize;

				ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
				viewSpace = mul(float32_t4(0.0f,0.0f,ndcDepth,1.0f),gOutline.projectionInverse);
				viewZ = viewSpace.z * rcp(viewSpace.w);
				if(viewZ <= gOutline.lengthChange && abs(viewZ - baseZ) >= 5){
					continue;
				}
				difference.x += viewZ * x * k;
				difference.y += viewZ * y * k;
			}
		}
	}

	float32_t weight = length(difference);
	weight = saturate(weight);

	output.color = gTexture.Sample(gSampler,input.texcoord);
	output.color.rgb = (1.0f - weight) * output.color.rgb;

	if(output.color.a == 0.0f){
		output.color.a = 0.0f;
	}
	else{
		output.color.a = 1.0f;
	}

	return output;
}