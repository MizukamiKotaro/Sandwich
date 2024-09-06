#include "Game.h"
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"
#include "GameElement/HitSystem/HitSystem.h"
#include "GameElement/ScoreManager/ScoreManager.h"

#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG


Game::Game(Camera* camera)
{
	input_ = Input::GetInstance();
	camera_ = camera;

	live_ = std::make_unique<LiveEditor>(camera_);
	music_ = std::make_unique<LiveMusics>();
	finishLive_ = std::make_unique<FinishLive>();
	
	notesList_ = NotesList::GetInstance();
	notesList_->Initialize();

	score_ = std::make_unique<Score>();

	stageUI_ = std::make_unique<StageUI>();
	stageUI_->SetIsMusicFinish(&music_->IsFinish());
	stageUI_->SetScore(score_.get());

	HitSystem::SetScore(score_.get());
}

void Game::Initialize()
{
	live_->Initialize();
	time_ = 0.0f;
	notesList_->PopCommands();

	score_->Reset();

	stageUI_->Initialize();

	HitSystem::Initialize();

	music_->Initialize();
	finishLive_->Initialize();
}

void Game::Update()
{
	music_->Update();
	float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	time_ += deltaTime;
	live_->Update(deltaTime);
	HitSystem::Update(time_);

#ifdef _DEBUG
	if (input_->PressedKey(DIK_E)) {
		if (isEdit_) {
			isEdit_ = false;
		}
		else {
			isEdit_ = true;
		}
	}
	ImGui::Begin("ステージシーン");
	ImGui::Checkbox("エディターモード", &isEdit_);
	if (isEdit_) {
		ImGui::Text("エディターモード ： オン");
	}
	else {
		ImGui::Text("エディターモード ： オフ");
	}
	ImGui::End();
#endif // _DEBUG


	if (music_->IsFinish()) {
#ifdef _DEBUG
		if (isEdit_) {
			return;
		}
#endif // _DEBUG
		finishLive_->Update(deltaTime);
		if (finishLive_->IsClear()) {
			ScoreManager::GetInstance()->SetClearScore(score_.get());
		}
	}
}

void Game::Draw()
{
	live_->Draw();
	notesList_->Draw(camera_);
	stageUI_->Draw();
}

void Game::FirstUpdate()
{
	live_->Update(0.00001f);
}
