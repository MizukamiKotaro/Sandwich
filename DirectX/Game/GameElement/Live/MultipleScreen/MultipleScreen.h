#pragma once
#include <memory>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Bloom/Bloom.h"
#include <unordered_map>
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;
class Screen;

class MultipleScreen {
public: 

	MultipleScreen();

	MultipleScreen(const std::string& chunkName);

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void PreDrawScene();

	void PostDrawScene();

	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle() const { return postEffect_->GetSRVGPUDescriptorHandle(); }

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void CreateScreen();

private:
	
	int screenNum_ = 1;

	std::unordered_map<int, std::unique_ptr<Screen>> screenMap_;
	std::unique_ptr<Sprite> screenSprite_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<PostEffect> postEffect1_;
	std::unique_ptr<Bloom> bloom_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
};
