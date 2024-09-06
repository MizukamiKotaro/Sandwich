#pragma once
#include <memory>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Bloom/Bloom.h"
#include <map>
#include "GameElement/IStageObject/IStageObject.h"
#include "SpotLightAndOutline/SpotLightAndOutline.h"

class ScreenEditor;

class MultipleScreenEditor : public IStageObject
{
public: 
	MultipleScreenEditor(const std::string& mainName, const std::string& name, const uint32_t& no);

	void Update(const float& time) override;
	void Draw(const Camera& camera) override;
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle() const { return postEffect_->GetSRVGPUDescriptorHandle(); }

	uint32_t& GetScreenNum() { return screenNum_; }
	std::map<uint32_t, std::unique_ptr<ScreenEditor>>* GetScreenMap() { return &screenMap_; }
	std::map<uint32_t, std::unique_ptr<SpotLightAndOutline>>* GetOutlineMap() { return &lightAndOutlineMap_; }

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;
	void CreateScreen();

private:
	
	uint32_t screenNum_ = 1;

	std::map<uint32_t, std::unique_ptr<ScreenEditor>> screenMap_;
	std::unique_ptr<Sprite> screenSprite_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<PostEffect> postEffect1_;
	std::unique_ptr<Bloom> bloom_;

	std::map<uint32_t, std::unique_ptr<SpotLightAndOutline>> lightAndOutlineMap_;

	struct TreeNames
	{
		std::string main;
		std::string name;
		std::string tree;
	};
	std::unique_ptr<TreeNames> treeName;
};
