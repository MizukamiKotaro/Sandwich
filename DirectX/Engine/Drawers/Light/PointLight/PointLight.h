#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "ILight/ILight.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"
#include "Drawers/IDrawer/IDrawer.h"

class Camera;
class ModelData;

class PointLight : public ILight, public IDrawer
{
public:
	struct PointLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // 位置
		float intensity; // 輝度
		float radius; // ライトの届く距離
		float decay; // 減衰率
		float padding[2];
	};

	PointLight();
	~PointLight() override;

	void Update() override;

	void Draw(const Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal) const;

public:
	PointLightData* light_ = nullptr;
	bool isDraw_;
};
