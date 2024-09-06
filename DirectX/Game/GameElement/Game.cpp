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
}

void Game::Update()
{
	// 時間差分
	//const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
}

void Game::Draw()
{

}

void Game::FirstUpdate()
{
}
