#pragma once
#include "Model.h"
#include <memory>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Bloom/Bloom.h"
#include "GameElement/IStageObject/IStageObject.h"

class Camera;

class ScreenEditor : public IStageObject
{
public:
	ScreenEditor(const std::string& mainName, const std::string& name, const uint32_t& no);
	ScreenEditor(const std::string& mainName, const std::string& name, const std::string& treeName, const uint32_t& no);
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;

	void PreDrawScene();
	void PostDrawScene();
	void SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle);
	void Reset();

	const Camera& GetCamera() const { return *camera_.get(); }

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;
	void CreateScreen();

	static const Vector3 kScale;
	float scale_ = 1.0f;
	
	std::unique_ptr<Model> screen_;
	std::unique_ptr<Sprite> screenSprite_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<Bloom> bloom_;

	std::unique_ptr<PostEffect> postEffect1_;

	std::unique_ptr<Camera> camera_;
};
