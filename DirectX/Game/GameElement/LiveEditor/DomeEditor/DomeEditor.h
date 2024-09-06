#pragma once
#include "Model.h"
#include <memory>
#include "GameElement/IStageObject/IStageObject.h"

class DomeEditor : public IStageObject
{
public: 
	DomeEditor(const std::string& mainName, const std::string& name, const uint32_t& no);

	void Initialize() override;
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;
	
	std::unique_ptr<Model> model_;
};
