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

struct HSVData {
	float32_t hue;
	float32_t saturate;
	float32_t value;
};
ConstantBuffer<HSVData> gHSV : register(b1);

float32_t3 RGBToHSV(float32_t3 rgb) {
	float32_t max = rgb.r > rgb.g ? rgb.r : rgb.g;
	max = max > rgb.b ? max : rgb.b;
	float32_t min = rgb.r < rgb.g ? rgb.r : rgb.g;
	min = min < rgb.b ? min : rgb.b;
	float32_t h = max - min;
	float32_t rcpH = rcp(h);
	if (h > 0.0f) {
    	if (max == rgb.r) {
        	h = (rgb.g - rgb.b) * rcpH;
        	if (h < 0.0f) {
            	h += 6.0f;
        	}
    	} else if (max == rgb.g) {
        	h = 2.0f + (rgb.b - rgb.r) * rcpH;
    	} else {
        	h = 4.0f + (rgb.r - rgb.g) * rcpH;
    	}
	}
	h /= 6.0f;
	float32_t s = (max - min);
	if (max != 0.0f){
		s *= rcp(max);
	}
	float32_t v = max;
	return float32_t3(h,s,v);
}

float32_t3 HSVToRGB(float32_t3 hsv) {
	float32_t r = hsv.b;
	float32_t g = hsv.b;
	float32_t b = hsv.b;
	if (hsv.g > 0.0f) {
    	hsv.r *= 6.0f;
    	int32_t i = (int32_t) hsv.r;
    	float32_t f = hsv.r - (float32_t) i;
    	switch (i) {
       	default:
        case 0:
            g *= 1 - hsv.g * (1 - f);
            b *= 1 - hsv.g;
            break;
        case 1:
            r *= 1 - hsv.g * f;
            b *= 1 - hsv.g;
            break;
        case 2:
            r *= 1 - hsv.g;
            b *= 1 - hsv.g * (1 - f);
            break;
        case 3:
            r *= 1 - hsv.g;
            g *= 1 - hsv.g * f;
            break;
        case 4:
            r *= 1 - hsv.g * (1 - f);
            g *= 1 - hsv.g;
            break;
        case 5:
            g *= 1 - hsv.g;
            b *= 1 - hsv.g * f;
            break;
    	}
	}
	return float32_t3(r,g,b);
}

float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange){
	float32_t range = maxRange - minRange;
	float32_t modValue = fmod(value - minRange, range);
	if(modValue < 0){
		modValue += range;
	}
	return minRange + modValue;
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	float32_t3 hsv = RGBToHSV(textureColor.rgb);

	hsv.x += gHSV.hue;
	hsv.y += gHSV.saturate;
	hsv.z += gHSV.value;

	hsv.x = WrapValue(hsv.x,0.0f,1.0f);
	hsv.y = saturate(hsv.y);
	hsv.z = saturate(hsv.z);

	float32_t3 rgb = HSVToRGB(hsv);
	output.color.rgb = rgb;
	output.color.a = textureColor.a;
	return output;
}