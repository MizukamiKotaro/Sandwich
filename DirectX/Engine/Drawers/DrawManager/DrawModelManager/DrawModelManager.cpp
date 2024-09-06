#include "DrawModelManager.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "Model.h"
#include "SkinningModel/SkinningModel.h"
#include "RigidAnimationModel/RigidAnimationModel.h"
#include "Texture/Texture.h"
#include "TextureManager.h"

const Texture* texCube = nullptr;

DrawModelManager::Transformation::Transformation()
{
	transformationResource = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData = nullptr;
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	*transformationData = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}

DrawModelManager::Transformation::~Transformation()
{
	transformationResource->Release();
}

DrawModelManager::DrawModelManager()
{
	texCube = TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");
	drawNum_ = 0;
	for (int32_t i = 0; i < 50; i++) {
		transformation_.push_back(std::make_unique<Transformation>());
	}
}

void DrawModelManager::Draw(const Model& model, const Camera& camera, const BlendMode& blendMode)
{
	if (transformation_.size() == drawNum_) {
		transformation_.push_back(std::make_unique<Transformation>());
	}

	transformation_[drawNum_]->transformationData->World = model.transform_.worldMat_;
	transformation_[drawNum_]->transformationData->WVP = model.transform_.worldMat_ * camera.GetViewProjection();
	transformation_[drawNum_]->transformationData->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(model.transform_.scale_)) *
		Matrix4x4::MakeRotateXYZMatrix(model.transform_.rotate_) * Matrix4x4::MakeTranslateMatrix(model.transform_.translate_);

	Draw(model, camera, blendMode, model.GetModelData().mesh.vertexBufferView_);
}

void DrawModelManager::Draw(const RigidAnimationModel& model, const Camera& camera, const BlendMode& blendMode)
{
	if (transformation_.size() == drawNum_) {
		transformation_.push_back(std::make_unique<Transformation>());
	}
	const QuaternionTransform& animTransform = model.GetAnimTransform();

	transformation_[drawNum_]->transformationData->World = animTransform.matrix_ * model.transform_.worldMat_;
	transformation_[drawNum_]->transformationData->WVP = animTransform.matrix_ * model.transform_.worldMat_ * camera.GetViewProjection();
	transformation_[drawNum_]->transformationData->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(animTransform.scale_) * Matrix4x4::MakeScaleMatrix(model.transform_.scale_)) *
		(Matrix4x4::MakeRotateMatrix(animTransform.rotate_) * Matrix4x4::MakeRotateXYZMatrix(model.transform_.rotate_)) * (Matrix4x4::MakeTranslateMatrix(animTransform.translate_) * Matrix4x4::MakeTranslateMatrix(model.transform_.translate_));

	Draw(model, camera, blendMode, model.GetModelData().mesh.vertexBufferView_);
}

void DrawModelManager::Draw(const SkinningModel& model, const Camera& camera, const BlendMode& blendMode)
{
	if (transformation_.size() == drawNum_) {
		transformation_.push_back(std::make_unique<Transformation>());
	}

	transformation_[drawNum_]->transformationData->World = model.transform_.worldMat_;
	transformation_[drawNum_]->transformationData->WVP = model.transform_.worldMat_ * camera.GetViewProjection();
	transformation_[drawNum_]->transformationData->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(model.transform_.scale_)) *
		Matrix4x4::MakeRotateXYZMatrix(model.transform_.rotate_) * Matrix4x4::MakeTranslateMatrix(model.transform_.translate_);

	Draw(model, camera, blendMode, model.GetSkinCluter().vertexBufferView);
}

void DrawModelManager::Draw(const BaseModel& model, const Camera& camera, const BlendMode& blendMode, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView)
{
	psoManager_->PreDraw(PipelineType::MODEL);
	psoManager_->SetBlendMode(PipelineType::MODEL, blendMode);
	const ModelData& modelData = model.GetModelData();
	const Light& light = model.GetLight();
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList_->IASetIndexBuffer(&modelData.mesh.indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(0, model.GetMaterialData().GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, transformation_[drawNum_]->transformationResource->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, model.GetTextureData());
	commandList_->SetGraphicsRootConstantBufferView(3, light.GetDirectionalLightGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(4, camera.GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(5, light.GetPointLightGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(6, light.GetSpotLightGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(7, texCube->handles_->gpuHandle);
	commandList_->DrawIndexedInstanced(UINT(modelData.mesh.indices.size()), 1, 0, 0, 0);
	drawNum_++;
}
