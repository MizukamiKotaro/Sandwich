#include "MultipleScreen.h"
#include "Camera.h"
#include "TextureManager.h"
#include "GameElement/Live/Screen/Screen.h"

MultipleScreen::MultipleScreen()
{
	screenSprite_ = std::make_unique<Sprite>("screen.png");
	screenSprite_->pos_ = { 640.0f,360.0f };
	screenSprite_->Update();

	postEffect_ = std::make_unique<PostEffect>();
	postEffect1_ = std::make_unique<PostEffect>();
	bloom_ = std::make_unique<Bloom>();

	globalVariable_ = std::make_unique<GlobalVariableUser>("Screen", "multipleScreen");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
	CreateScreen();
}

MultipleScreen::MultipleScreen(const std::string& chunkName)
{
	screenSprite_ = std::make_unique<Sprite>("screen.png");
	screenSprite_->pos_ = { 640.0f,360.0f };
	screenSprite_->Update();

	postEffect_ = std::make_unique<PostEffect>();
	postEffect1_ = std::make_unique<PostEffect>();
	bloom_ = std::make_unique<Bloom>();

	globalVariable_ = std::make_unique<GlobalVariableUser>(chunkName, "multipleScreen");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
	CreateScreen();
}

void MultipleScreen::Initialize()
{
}

void MultipleScreen::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	CreateScreen();
	for (int i = 0; i < screenNum_; i++) {
		screenMap_[i]->Update();
	}
#endif // _DEBUG
}

void MultipleScreen::Draw(Camera* camera)
{
	for (int i = 0; i < screenNum_; i++) {
		screenMap_[i]->Draw(camera);
	}
}

void MultipleScreen::PreDrawScene()
{
	//bloom_->PreDrawScene();
	postEffect1_->PreDrawScene();
}

void MultipleScreen::PostDrawScene()
{
	//bloom_->PostDrawScene();
	postEffect1_->PostDrawScene();

	postEffect_->PreDrawScene();

	//bloom_->Draw();
	postEffect1_->Draw();
	screenSprite_->Draw(BlendMode::kBlendModeMultiply);

	postEffect_->PostDrawScene();
}

void MultipleScreen::SetGlobalVariable()
{
	globalVariable_->AddItem("スクリーンの数", screenNum_);
	ApplyGlobalVariable();
}

void MultipleScreen::ApplyGlobalVariable()
{
	screenNum_ = globalVariable_->GetIntValue("スクリーンの数");

	if (screenNum_ <= 0) {
		screenNum_ = 1;
	}
}

void MultipleScreen::CreateScreen()
{
	for (int i = 0; i < screenNum_; i++) {
		if (screenMap_.find(i) == screenMap_.end()) {
			screenMap_[i] = std::make_unique<Screen>(globalVariable_->GetChunkName(), "Screen" + std::to_string(i));
			screenMap_[i]->SetGPUHandle(postEffect_->GetSRVGPUDescriptorHandle());
		}
	}
}
