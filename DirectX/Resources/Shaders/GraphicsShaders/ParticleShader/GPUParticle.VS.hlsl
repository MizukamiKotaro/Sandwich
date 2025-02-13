#include "Particle.hlsli"

struct Particle {
	float32_t3 translate;
	float32_t3 scale;
	float32_t3 rotate;
	float32_t lifeTime;
	float32_t3 velocity;
	float32_t currentTime;
	float32_t4 color;
};
StructuredBuffer<Particle>gParticles : register(t0);

struct PerView {
	float32_t4x4 viewProjection;
	float32_t4x4 billboardMatrix;
};
ConstantBuffer<PerView> gPerView : register(b0);

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceID : SV_InstanceID) {
	VertexShaderOutput output;
	Particle particle = gParticles[instanceID];
	float32_t4x4 worldMatrix = gPerView.billboardMatrix;
	worldMatrix[0] *= particle.scale.x;
	worldMatrix[1] *= particle.scale.y;
	worldMatrix[2] *= particle.scale.z;
	worldMatrix[3].xyz = particle.translate;

	output.vertexPos = mul(input.vertexPos, mul(worldMatrix, gPerView.viewProjection));
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)worldMatrix));
	output.color = particle.color;
	return output;
}