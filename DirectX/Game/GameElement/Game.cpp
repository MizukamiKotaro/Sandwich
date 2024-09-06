#include "Game.h"
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"

#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG


Game::Game(Camera* camera)
{
	// 入力マネージャ
	input_ = Input::GetInstance();
	// カメラの参照
	camera_ = camera;
}

void Game::Initialize()
{
	player_ = std::make_unique<Player>();
	player_->Init();
}

void Game::Update()
{
	// 時間差分
	//const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	player_->Update();
}

void Game::Draw()
{
	player_->Draw(camera_);
}

void Game::FirstUpdate()
{
}
