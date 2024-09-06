#include "Skybox.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
};
ConstantBuffer<TransformationMatrix>gTransformationMatrix : register(b0);

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.vertexPos = mul(input.vertexPos, gTransformationMatrix.WVP).xyww;
	output.texcoord = input.vertexPos.xyz;
	return output;
}