#include "IDrawManager.h"
#include "DirectXBase/DirectXBase.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

ID3D12GraphicsCommandList* IDrawManager::commandList_ = nullptr;
GraphicsPipelineManager* IDrawManager::psoManager_ = nullptr;

void IDrawManager::StaticInitialize()
{
	commandList_ = DirectXBase::GetInstance()->GetCommandList();
	psoManager_ = GraphicsPipelineManager::GetInstance();
}

void IDrawManager::Reset()
{
	drawNum_ = 0;
}
