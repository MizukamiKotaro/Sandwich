#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"

StageScene::StageScene()
{
	FirstInit();
	camera_->transform_.translate_.z = -100.0f;
	camera_->Update();
	game_ = std::make_unique<Game>(camera_.get());

}

void StageScene::Initialize()
{
	game_->Initialize();
}

void StageScene::Update()
{
	if (game_->GetIsClear()) {
		// シーン切り替え
		ChangeScene(CLEAR);
		Audio::AllStop();
	}

#ifdef _DEBUG
	if (input_->PressedKey(DIK_1)) {
		// シーン切り替え
		ChangeScene(CLEAR);
		Audio::AllStop();
	}
#endif // _DEBUG
	camera_->Update();
	game_->Update();
}

void StageScene::Draw()
{
	Kyoko::Engine::PreDraw();

	game_->Draw();

	BlackDraw();

	// フレームの終了
	Kyoko::Engine::PostDraw();
}

void StageScene::FirstUpdate()
{
	game_->FirstUpdate();
}

