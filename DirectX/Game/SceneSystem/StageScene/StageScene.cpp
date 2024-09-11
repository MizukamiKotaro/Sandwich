#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"
#include "FrameInfo/FrameInfo.h"

StageScene::StageScene()
{
	FirstInit();
	camera_->transform_.translate_.z = -100.0f;
	camera_->Update();
	game_ = std::make_unique<Game>(camera_.get());
	backGround_ = std::make_unique<BackGround>();
	bgm_ = std::make_unique<Audio>();
	bgm_->Load("game.mp3", "ゲームBGM");
	bgm_->Play(true);
	score_ = Score::GetInstance();
	score_->FirstInitialize();
}

void StageScene::Initialize()
{
	game_->Initialize();
	backGround_->Initialise();
	score_->Initialize();
	score_->Update(0.0f);
}

void StageScene::Update()
{
	if (game_->GetIsClear()) {
		// シーン切り替え
		ChangeScene(CLEAR);
		Audio::AllStop();
	}
	if (backGround_->GetIsReset()) {
		Initialize();
	}

#ifdef _DEBUG
	if (input_->PressedKey(DIK_N)) {
		// シーン切り替え
		ChangeScene(CLEAR);
		Audio::AllStop();
	}
	if (input_->PressedKey(DIK_R)) {
		Initialize();
	}
#endif // _DEBUG
	float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();

	score_->Update(deltaTime);
	backGround_->Update(deltaTime);
	camera_->Update();
	game_->Update();
}

void StageScene::Draw()
{
	Kyoko::Engine::PreDraw();
	backGround_->Draw(*camera_.get());
	game_->Draw();
	score_->Draw();

	BlackDraw();

	// フレームの終了
	Kyoko::Engine::PostDraw();
}

void StageScene::FirstUpdate()
{
	game_->FirstUpdate();
}

