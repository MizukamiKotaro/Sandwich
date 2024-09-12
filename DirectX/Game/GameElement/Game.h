#pragma once
#include <memory>
#include "Camera.h"
#include "Audio.h"
#include "Input.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "Player/Player.h"
#include "Equipment/EquipmentManager.h"
#include "Customer/Customer.h"
#include "Particle/Eat/EatParticle.h"

class GameManager;

class Game {
public: 
	Game(Camera* camera);

	void Initialize();
	void ResetInitialize();
	void Update();
	void Draw();
	void FirstUpdate();
	const bool& GetIsClear() const { return IsClear; }

private:
	std::unique_ptr<Player> player_;
	std::unique_ptr<EatParticle> eatParticle_;
	//お客さんクラス
	std::unique_ptr<Customer> customer_;
	EquipmentManager* equipmentManager_;
	InstancingModelManager* instancingModelManager_;
	ParticleManager* particleManager_;
private:
	GameManager* gameManager_;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	CollisionManager* collisionManager_ = nullptr;

private:
	bool IsClear = false;
};
