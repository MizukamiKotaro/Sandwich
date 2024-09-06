#include "DrawLightManager.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "SpotLight/SpotLight.h"
#include "PointLight/PointLight.h"
#include "calc.h"

DrawLightManager::Transformation::Transformation()
{
	transformationResource = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData = nullptr;
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	*transformationData = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}

DrawLightManager::Transformation::~Transformation()
{
	transformationResource->Release();
}

DrawLightManager::DrawLightManager()
{
	drawNum_ = 0;
	scaleMat_ = Matrix4x4::MakeScaleMatrix({ 1000.0f,1000.0f,1000.0f });
	scaleInverseMat_ = Matrix4x4::Inverse(scaleMat_);
	modelData_ = ModelDataManager::GetInstance()->LoadObj("Plane");
	for (int32_t i = 0; i < 50; i++) {
		transformation_.push_back(std::make_unique<Transformation>());
	}
}

void DrawLightManager::Draw(const PointLight& light, const Camera& camera, const BlendMode& blendMode)
{
	Matrix4x4 billboardMat{};

	billboardMat = camera.transform_.worldMat_;
	billboardMat.m[3][0] = 0.0f;
	billboardMat.m[3][1] = 0.0f;
	billboardMat.m[3][2] = 0.0f;

	Matrix4x4 translateMat = Matrix4x4::MakeTranslateMatrix(light.light_->position + Vector3{ 0.0f,0.0f,0.1f } * billboardMat);

	if (transformation_.size() == drawNum_) {
		transformation_.push_back(std::make_unique<Transformation>());
	}
	transformation_[drawNum_]->transformationData->World = scaleMat_ * billboardMat * translateMat;
	transformation_[drawNum_]->transformationData->WVP = transformation_[drawNum_]->transformationData->World * camera.GetViewProjection();
	transformation_[drawNum_]->transformationData->WorldInverse = scaleInverseMat_ * billboardMat * translateMat;

	PipelineType piplineType = PipelineType::SPOT_LIGHT;
	psoManager_->PreDraw(piplineType);
	psoManager_->SetBlendMode(piplineType, blendMode);
	commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_);
	commandList_->IASetIndexBuffer(&modelData_->mesh.indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(1, transformation_[drawNum_]->transformationResource->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(2, camera.GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(0, light.GetGPUVirtualAddress());
	commandList_->DrawIndexedInstanced(UINT(modelData_->mesh.indices.size()), 1, 0, 0, 0);

	drawNum_++;
}

void DrawLightManager::Draw(const SpotLight& light, const Camera& camera, const BlendMode& blendMode)
{
	Matrix4x4 billboardMat{};
	billboardMat = camera.transform_.worldMat_;
	billboardMat.m[3][0] = 0.0f;
	billboardMat.m[3][1] = 0.0f;
	billboardMat.m[3][2] = 0.0f;

	Vector3 project = Calc::Project(camera.transform_.worldPos_ - light.light_->position, light.light_->direction);
	Vector3 rotate = camera.transform_.worldPos_ - (light.light_->position + project);
	rotate = rotate.Normalize();
	if (rotate.z != -1.0f) {
		billboardMat = billboardMat * Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,-1.0f } * Matrix4x4::MakeRotateXYZMatrix(camera.transform_.rotate_), rotate);
	}
	Matrix4x4 translateMat = Matrix4x4::MakeTranslateMatrix(light.light_->position + Vector3{ 0.0f,0.0f,0.1f } * billboardMat);

	if (transformation_.size() == drawNum_) {
		transformation_.push_back(std::make_unique<Transformation>());
	}
	transformation_[drawNum_]->transformationData->World = scaleMat_ * billboardMat * translateMat;
	transformation_[drawNum_]->transformationData->WVP = transformation_[drawNum_]->transformationData->World * camera.GetViewProjection();
	transformation_[drawNum_]->transformationData->WorldInverse = scaleInverseMat_ * billboardMat * translateMat;

	PipelineType piplineType = PipelineType::SPOT_LIGHT;
	psoManager_->PreDraw(piplineType);
	psoManager_->SetBlendMode(piplineType, blendMode);
	commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_);
	commandList_->IASetIndexBuffer(&modelData_->mesh.indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(1, transformation_[drawNum_]->transformationResource->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(2, camera.GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(0, light.GetGPUVirtualAddress());
	commandList_->DrawIndexedInstanced(UINT(modelData_->mesh.indices.size()), 1, 0, 0, 0);

	psoManager_->PreDraw(PipelineType::SPOT_LIGHT_DEPTH);
	psoManager_->SetBlendMode(PipelineType::SPOT_LIGHT_DEPTH, BlendMode::kBlendModeAdd);
	commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_);
	commandList_->IASetIndexBuffer(&modelData_->mesh.indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(1, transformation_[drawNum_]->transformationResource->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(2, camera.GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(0, light.GetGPUVirtualAddress());
	commandList_->DrawIndexedInstanced(UINT(modelData_->mesh.indices.size()), 1, 0, 0, 0);

	drawNum_++;
}
