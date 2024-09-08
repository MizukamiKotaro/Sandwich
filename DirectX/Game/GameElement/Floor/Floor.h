#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"

class Floor : public Collider {
public:
	Floor(Vector3 position);
	//void Init();
	void Update();
	void Draw(const Camera* camera);

	//当たり判定の更新
	void ColliderUpdate();
	//当たり判定
	void OnCollision(const Collider& collider)override;

private:
	//3Dモデル
	std::unique_ptr<Object> object_;
};