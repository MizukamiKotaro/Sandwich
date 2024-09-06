struct Well{
	float32_t4x4 skeletonSpaceMatrix;
	float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};
StructuredBuffer<Well> gMatrixPalette : register(t0);

struct Vertex {
	float32_t4 position;
	float32_t2 texcoord;
	float32_t3 normal;
};
StructuredBuffer<Vertex> gInputVerteces : register(t1);

struct VertexInfluence {
	float32_t4 weight;
	int32_t4 index;
};
StructuredBuffer<VertexInfluence> gInfluence : register(t2);

struct SkinningInformation {
	uint32_t numVertices;
};
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

RWStructuredBuffer<Vertex> gOutputVerteces : register(u0);

[numthreads(1024,1,1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{
	uint32_t vertexIndex = DTid.x;
	if(vertexIndex < gSkinningInformation.numVertices) {
		Vertex input = gInputVerteces[vertexIndex];
		VertexInfluence influence = gInfluence[vertexIndex];

		Vertex skinned;

		skinned.texcoord = input.texcoord;

		skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
		skinned.position.w = 1.0f;

		skinned.normal = mul(input.normal,(float32_t3x3)gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
		skinned.normal += mul(input.normal,(float32_t3x3)gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
		skinned.normal += mul(input.normal,(float32_t3x3)gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
		skinned.normal += mul(input.normal,(float32_t3x3)gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
		skinned.normal = normalize(skinned.normal);

		gOutputVerteces[vertexIndex] = skinned;
	}
}