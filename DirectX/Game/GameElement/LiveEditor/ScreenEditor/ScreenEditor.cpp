#include "ScreenEditor.h"
#include "Camera.h"
#include "TextureManager.h"
#include <numbers>

const Vector3 ScreenEditor::kScale = { 3.2f * 1.4f,1.8f * 1.4f,1.0f };

ScreenEditor::ScreenEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	CreateScreen();
	CreateStageEditor(mainName, name, no);
	camera_ = std::make_unique<Camera>();
	SetGlobalVariable();
}

ScreenEditor::ScreenEditor(const std::string& mainName, const std::string& name, const std::string& treeName, const uint32_t& no)
{
	CreateScreen();
	CreateStageEditor(mainName, name, treeName, no);
	camera_ = std::make_unique<Camera>();
	SetGlobalVariable();
}

void ScreenEditor::Update(const float& time) {
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG
	if (time) {}
	camera_->Update();
}

void ScreenEditor::Draw(const Camera& camera) 
{
	screen_->Draw(camera);
}

void ScreenEditor::PreDrawScene()
{
	postEffect1_->PreDrawScene();
}

void ScreenEditor::PostDrawScene()
{
	//bloom_->PostDrawScene();
	postEffect1_->PostDrawScene();

	postEffect_->PreDrawScene();

	//bloom_->Draw();
	postEffect1_->Draw();
	screenSprite_->Draw(BlendMode::kBlendModeMultiply);

	postEffect_->PostDrawScene();
}

void ScreenEditor::SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle)
{
	screen_->SetSRVGPUDescriptorHandle_(srvGPUDescriptorHandle);
}

void ScreenEditor::Reset()
{
	screen_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
}

void ScreenEditor::SetGlobalVariable()
{
	stageEditor_->AddItem("スケール", scale_);
	stageEditor_->AddItem("ポジション", screen_->transform_.translate_);
	stageEditor_->AddItem("向き", screen_->transform_.rotate_);

	stageEditor_->AddItem("ポジション", Vector3{ 0.0f,6.0f,-40.0f }, "カメラ");
	stageEditor_->AddItem("角度", Vector3{ 0.1f,0.0f,0.0f }, "カメラ");

	ApplyGlobalVariable();
}

void ScreenEditor::ApplyGlobalVariable()
{
	scale_ = stageEditor_->GetFloatValue("スケール");
	screen_->transform_.translate_ = stageEditor_->GetVector3Value("ポジション");
	screen_->transform_.rotate_ = stageEditor_->GetVector3Value("向き");

	camera_->transform_.translate_ = stageEditor_->GetVector3Value("ポジション", "カメラ");
	camera_->transform_.rotate_ = stageEditor_->GetVector3Value("角度", "カメラ");

	screen_->transform_.scale_ = kScale * scale_;
	screen_->Update();
	camera_->Update();
}

void ScreenEditor::CreateScreen()
{
	screen_ = std::make_unique<Model>("plane");
	screen_->SetTexture(TextureManager::GetInstance()->LoadTexture("Resources/Texture/white.png"));
	screen_->transform_.translate_ = { 0.0f,4.0f,4.0f };
	screen_->transform_.scale_ = kScale;
	screen_->Update();
	screen_->UnUsedLight();
	screen_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });

	screenSprite_ = std::make_unique<Sprite>("screen.png");
	screenSprite_->pos_ = { 640.0f,360.0f };
	screenSprite_->Update();

	postEffect_ = std::make_unique<PostEffect>();
	postEffect1_ = std::make_unique<PostEffect>();
	bloom_ = std::make_unique<Bloom>();

	screen_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
}
