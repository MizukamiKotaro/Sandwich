#pragma once
#include <memory>
#include "Camera.h"
#include "Audio.h"
#include "Input.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "Player/Player.h"
#include "Equipment/EquipmentManager.h"
#include "Customer/Customer.h"

class GameManager;

class Game {
public: 
	Game(Camera* camera);

	void Initialize();
	void Update();
	void Draw();
	void FirstUpdate();
	const bool& GetIsClear() const { return IsClear; }

private:
	std::unique_ptr<Player> player_;
	//お客さんクラス
	std::unique_ptr<Customer> customer_;
	EquipmentManager* equipmentManager_;
	InstancingModelManager* instancingModelManager_;
private:
	GameManager* gameManager_;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	CollisionManager* collisionManager_ = nullptr;

private:
	bool IsClear = false;
};
