#include "Game.h"
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"

#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

#include "GameManager/GameManager.h"

Game::Game(Camera* camera)
{
	gameManager_ = GameManager::GetInstance();
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
	equipmentManager_->SetPlayer(player_.get());

	customer_ = std::make_unique<Customer>();
	customer_->Init();

	gameManager_->ChangeScene(GameManager::kTitle);
	gameManager_->CompletedTransition();
}

void Game::Update()
{
	if (player_->GetIsDrop() && gameManager_->GetScene() == GameManager::kTitle) {
		gameManager_->ChangeScene(GameManager::kGame);
		// これ仮置き
		gameManager_->CompletedTransition();
		customer_->isDraw = true;
	}

	collisionManager_->Clear();
	// 時間差分
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	equipmentManager_->Update(deltaTime);

	player_->Update();

	customer_->Update();

	collisionManager_->CheckCollision();
}

void Game::Draw()
{
	instancingModelManager_->Clear();

	equipmentManager_->Draw();
	player_->Draw(camera_);
	instancingModelManager_->Draw(*camera_);
	customer_->Draw(camera_);
}

void Game::FirstUpdate()
{
}
