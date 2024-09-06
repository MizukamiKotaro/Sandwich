#include "DrawParticleManager.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "Drawers/GPUParticle/GPUParticle.h"
#include "Base/DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

DrawParticleManager::DrawParticleManager()
{
	drawNum_ = 0;
	modelData_ = ModelDataManager::GetInstance()->LoadObj("Plane");
}

void DrawParticleManager::Draw(const GPUParticle& particle, const Camera& camera, const BlendMode& blendMode)
{
	if (!camera_) {
		camera_ = &camera;
		if (perViews_.empty()) {
			perViews_.push_back(std::make_unique<PerView>());
		}
		SetPerView();
	}
	else if (camera_ != &camera) {
		drawNum_++;
		camera_ = &camera;
		if (perViews_.size() == drawNum_) {
			perViews_.push_back(std::make_unique<PerView>());
		}
		SetPerView();
	}


	PipelineType piplineType = PipelineType::GPU_PARTICLE;
	psoManager_->PreDraw(piplineType);
	psoManager_->SetBlendMode(piplineType, blendMode);
	commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_);
	commandList_->IASetIndexBuffer(&modelData_->mesh.indexBufferView_);
	commandList_->SetGraphicsRootConstantBufferView(0, perViews_[drawNum_]->perViewResource->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(1, particle.GetParticleGPUDescriptorHandle());
	commandList_->SetGraphicsRootDescriptorTable(2, particle.GetTexture()->handles_->gpuHandle);

	commandList_->DrawIndexedInstanced(UINT(modelData_->mesh.indices.size()), 1024, 0, 0, 0);

	drawNum_++;
}

void DrawParticleManager::SetPerView()
{
	perViews_[drawNum_]->perViewData->billboardMatrix = camera_->transform_.worldMat_;
	perViews_[drawNum_]->perViewData->billboardMatrix.m[3][0] = 0.0f;
	perViews_[drawNum_]->perViewData->billboardMatrix.m[3][1] = 0.0f;
	perViews_[drawNum_]->perViewData->billboardMatrix.m[3][2] = 0.0f;
	perViews_[drawNum_]->perViewData->viewProjection = camera_->GetViewProjection();
}

DrawParticleManager::PerView::PerView()
{
	perViewResource = DirectXBase::CreateBufferResource(sizeof(PerViewData));
	perViewData = nullptr;
	perViewResource->Map(0, nullptr, reinterpret_cast<void**>(&perViewData));
}

DrawParticleManager::PerView::~PerView()
{
	perViewResource->Release();
}
