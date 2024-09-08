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

	collisionManager_ = CollisionManager::GetInstance();
	equipmentManager_ = EquipmentManager::GetInstance();
	equipmentManager_->FirstInitialize();
	instancingModelManager_ = InstancingModelManager::GetInstance();
}

void Game::Initialize()
{
	equipmentManager_->Initialize();
	player_ = std::make_unique<Player>();
	player_->Init();
}

void Game::Update()
{
	collisionManager_->Clear();
	// 時間差分
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	equipmentManager_->Update(deltaTime);

	player_->Update();

	collisionManager_->CheckCollision();
}

void Game::Draw()
{
	instancingModelManager_->Clear();

	equipmentManager_->Draw();
	player_->Draw(camera_);
	instancingModelManager_->Draw(*camera_);
}

void Game::FirstUpdate()
{
}
