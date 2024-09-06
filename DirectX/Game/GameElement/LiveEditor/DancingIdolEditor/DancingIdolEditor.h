#pragma once
#include "SkinningModel/SkinningModel.h"
#include "GameElement/IStageObject/IStageObject.h"
#include <memory>

class DancingIdolEditor : public IStageObject
{
public: 

	DancingIdolEditor(const std::string& mainName, const std::string& name, const uint32_t& no);
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;
	
	std::unique_ptr<SkinningModel> idol_;
	Vector3 firstPosition_;
};
