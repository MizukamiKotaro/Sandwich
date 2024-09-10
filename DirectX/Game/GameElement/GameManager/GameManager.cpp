#include "GameManager.h"

GameManager* GameManager::GetInstance()
{
	static GameManager instance;
	return &instance;
}

void GameManager::Initialize()
{
	state_ = kTitle;
	stageNum_ = 0;
	isTransition_ = false;
}

void GameManager::ChangeScene(const int32_t& nextScene)
{
	state_ = nextScene;
	isTransition_ = true;
}

void GameManager::CompletedTransition()
{
	isTransition_ = false;
}

const int32_t& GameManager::GetScene() const
{
	return state_;
}

const bool& GameManager::GetIsTransition() const
{
	return isTransition_;
}
