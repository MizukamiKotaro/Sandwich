#include "Skybox.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "Texture.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Skybox::Skybox(const std::string& filename, const Vector3& scale, const Vector3& rotate, const Vector3& position, const Vector4& color)
{
	LoadTexture(filename);
	CreateVertexRes();
	CreateMaterialRes();
	CreateTranformRes();

	scale_ = scale;
	rotate_ = rotate;
	position_ = position;
	worldMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
	materialData_->color = color;
}

Skybox::~Skybox()
{
	vertexResource_->Release();
	transformResource_->Release();
	materialResource_->Release();
	indexResource_->Release();
}

void Skybox::Update()
{
	worldMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
}

void Skybox::Draw(const Camera& camera)
{
	psoManager_->PreDraw(PipelineType::SKYBOX);

	transformData_->WVP = worldMat_ * camera.GetViewProjection();
	
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList_->IASetIndexBuffer(&indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, texture_->handles_->gpuHandle);
	commandList_->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void Skybox::LoadTexture(const std::string& filePath)
{
	texture_ = textureManager_->LoadTexture(filePath);
}

void Skybox::SetTexture(const Texture* texture)
{
	texture_ = texture;
}

void Skybox::SetColor(const Vector4& color)
{
	materialData_->color = color;
}

void Skybox::CreateVertexRes()
{
	vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * 8);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 8;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0].vertexPos = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[1].vertexPos = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[2].vertexPos = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[3].vertexPos = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[4].vertexPos = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[5].vertexPos = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[6].vertexPos = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[7].vertexPos = { -1.0f,-1.0f,1.0f,1.0f };

	indexResource_ = DirectXBase::CreateBufferResource(sizeof(uint32_t) * 36);
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 36);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	mappedIndex = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	
	mappedIndex[0] = 0; mappedIndex[1] = 1; mappedIndex[2] = 2;
	mappedIndex[3] = 2; mappedIndex[4] = 1; mappedIndex[5] = 3;
	mappedIndex[6] = 4; mappedIndex[7] = 5; mappedIndex[8] = 6;
	mappedIndex[9] = 6; mappedIndex[10] = 5; mappedIndex[11] = 7;
	mappedIndex[12] = 5; mappedIndex[13] = 0; mappedIndex[14] = 7;
	mappedIndex[15] = 7; mappedIndex[16] = 0; mappedIndex[17] = 2;
	mappedIndex[18] = 1; mappedIndex[19] = 4; mappedIndex[20] = 3;
	mappedIndex[21] = 3; mappedIndex[22] = 4; mappedIndex[23] = 6;
	mappedIndex[24] = 4; mappedIndex[25] = 1; mappedIndex[26] = 5;
	mappedIndex[27] = 5; mappedIndex[28] = 1; mappedIndex[29] = 0;
	mappedIndex[30] = 7; mappedIndex[31] = 2; mappedIndex[32] = 6;
	mappedIndex[33] = 6; mappedIndex[34] = 2; mappedIndex[35] = 3;
}

void Skybox::CreateMaterialRes()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分を用意する
	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));
	//マテリアルデータを書き込む
	//書き込むためのアドレスを取得\l
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は赤を書き込んでいる
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
}

void Skybox::CreateTranformRes()
{
	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	//書き込むためのアドレスを取得
	transformResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));
	//単位行列を書き込んでいく
	transformData_->WVP = { Matrix4x4::MakeIdentity4x4() };
}