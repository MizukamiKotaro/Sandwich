#pragma once
#include <array>
#include <memory>
#include "GameElement/IStageObject/IStageObject.h"

class SpotLightBoxAnimationEditor;

class TwinSpotLightBoxAnimationEditor : public IStageObject
{
public:
	TwinSpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const uint32_t& no);
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;
	void DrawLight(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;

public:
	std::array<std::unique_ptr<SpotLightBoxAnimationEditor>, 2> lights_;
};