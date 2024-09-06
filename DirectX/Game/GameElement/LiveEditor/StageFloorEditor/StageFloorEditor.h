#pragma once
#include "Model.h"
#include <memory>
#include "GameElement/IStageObject/IStageObject.h"
#include "Noise/Noise.h"
#include "PostEffect/PostEffect.h"

class StageFloorEditor : public IStageObject
{
public:
	StageFloorEditor(const std::string& mainName, const std::string& name, const uint32_t& no);

	void Initialize() override;
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;
	void DrawSub(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;

	void WaterInitialize();
	void WaterUpdate(const float& time);

	std::unique_ptr<Model> model_;
	std::unique_ptr<PostEffect> post_;
	std::unique_ptr<Noise> noise_;
	std::unique_ptr<Model> stage_;
	uint32_t uneune_;
	bool isWater_;
	bool isStage_;
};
