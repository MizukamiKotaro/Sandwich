#pragma once
#include "Model.h"
#include <memory>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Bloom/Bloom.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;

class Screen {
public: 

	Screen();
	Screen(const std::string& chunkName, const std::string& groupName);

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void PreDrawScene();

	void PostDrawScene();

public:

	void SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle);

	void Reset();

	void CreateGroup();

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:

	static const Vector3 kScale;
	float scale_ = 1.0f;
	
	std::unique_ptr<Model> screen_;
	std::unique_ptr<Sprite> screenSprite_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<Bloom> bloom_;

	std::unique_ptr<PostEffect> postEffect1_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
};
