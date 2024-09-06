#pragma once
#include <d3d12.h>
#include <stdint.h>
class GraphicsPipelineManager;

class IDrawManager {
public:
	static void StaticInitialize();
	void Reset();
protected:
	static ID3D12GraphicsCommandList* commandList_;
	static GraphicsPipelineManager* psoManager_;
	int32_t drawNum_;
};