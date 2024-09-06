#include "BaseModel.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "Camera.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "calc.h"
#include "DescriptorHeapManager/DescriptorHeapManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

BaseModel::~BaseModel()
{
	transformationResource_->Release();
	materialResource_->Release();
}

void BaseModel::SetTexture(const Texture* texture)
{
	texture_ = texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;
}

void BaseModel::SetModelData(const ModelData* modelData)
{
	modelData_ = modelData;

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;
}

void BaseModel::SetLight(const ILight* light)
{
	light_.SetLight(light);
}

void BaseModel::SetColor(const Vector4& color)
{
	color_ = color;
	materialData_->color = color_;
}

const Matrix4x4 BaseModel::GetRotateMatrix()
{
	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_);
}

void BaseModel::CreateModel(const std::string& fileName)
{
	modelData_ = modelDataManager_->LoadObj(fileName);

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	CreateResources();

	InitVariables();
}

void BaseModel::CreateModel(const ModelData* modelData)
{
	modelData_ = modelData;

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	CreateResources();

	InitVariables();
}

void BaseModel::CreateResources()
{
	CreateMaterialResource();

	CreateTransformationResource();
}

void BaseModel::CreateMaterialResource()
{
	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = 3;
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
	materialData_->shininess = 40.0f;
	materialData_->supeqularColor = { 1.0f, 1.0f, 1.0f };
	materialData_->skyboxAlpha = 0.4f;
}

void BaseModel::CreateTransformationResource()
{
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData_ = nullptr;
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	*transformationData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}

void BaseModel::InitVariables()
{
	light_.Initialize();

	transform_ = EulerTransform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void BaseModel::TransformUpdate()
{
	transform_.UpdateMatrix();
	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
	materialData_->color = color_;
}
