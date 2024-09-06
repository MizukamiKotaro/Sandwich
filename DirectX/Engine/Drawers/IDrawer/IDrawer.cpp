#include "IDrawer.h"
#include "DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "TextureManager.h"
#include "Drawers/DrawManager/DrawManager.h"

ID3D12GraphicsCommandList* IDrawer::commandList_ = nullptr;
GraphicsPipelineManager* IDrawer::psoManager_ = nullptr;
ModelDataManager* IDrawer::modelDataManager_ = nullptr;
TextureManager* IDrawer::textureManager_ = nullptr;
DrawManager* IDrawer::drawManager_ = nullptr;

void IDrawer::StaticInitialize()
{
	commandList_ = DirectXBase::GetInstance()->GetCommandList();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	modelDataManager_ = ModelDataManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	drawManager_ = DrawManager::GetInstance();
	drawManager_->Initialize();
}