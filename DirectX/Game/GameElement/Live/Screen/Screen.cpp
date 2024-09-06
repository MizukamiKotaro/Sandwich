#include "Screen.h"
#include "Camera.h"
#include "TextureManager.h"
#include <numbers>

const Vector3 Screen::kScale = { 3.2f * 1.4f,1.8f * 1.4f,1.0f };

Screen::Screen()
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

	globalVariable_ = std::make_unique<GlobalVariableUser>("Screen", "Screen");
}

Screen::Screen(const std::string& chunkName, const std::string& groupName)
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

	globalVariable_ = std::make_unique<GlobalVariableUser>(chunkName, groupName);
	globalVariable_->CreateGroup();
	SetGlobalVariable();
}

void Screen::Initialize() {
	
}

void Screen::Update() {
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

void Screen::Draw(Camera* camera) {
	screen_->Draw(*camera);
}

void Screen::PreDrawScene()
{
	//bloom_->PreDrawScene();
	postEffect1_->PreDrawScene();
}

void Screen::PostDrawScene()
{
	//bloom_->PostDrawScene();
	postEffect1_->PostDrawScene();

	postEffect_->PreDrawScene();

	//bloom_->Draw();
	postEffect1_->Draw();
	screenSprite_->Draw(BlendMode::kBlendModeMultiply);

	postEffect_->PostDrawScene();
}

void Screen::SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle)
{
	screen_->SetSRVGPUDescriptorHandle_(srvGPUDescriptorHandle);
}

void Screen::Reset()
{
	screen_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
}

void Screen::CreateGroup()
{
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void Screen::SetGlobalVariable()
{
	if (globalVariable_) {
		globalVariable_->AddItem("スケール", scale_);
		globalVariable_->AddItem("ポジション", screen_->transform_.translate_);
		globalVariable_->AddItem("向き", screen_->transform_.rotate_);

		ApplyGlobalVariable();
	}
}

void Screen::ApplyGlobalVariable()
{
	if (globalVariable_) {
		scale_ = globalVariable_->GetFloatValue("スケール");
		screen_->transform_.translate_ = globalVariable_->GetVector3Value("ポジション");
		screen_->transform_.rotate_ = globalVariable_->GetVector3Value("向き");

		screen_->transform_.scale_ = kScale * scale_;
		screen_->Update();
	}
}