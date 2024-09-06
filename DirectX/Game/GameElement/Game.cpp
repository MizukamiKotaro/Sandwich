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
}

void Game::Initialize()
{
}

void Game::Update()
{
	float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
}

void Game::Draw()
{

}

void Game::FirstUpdate()
{
}
