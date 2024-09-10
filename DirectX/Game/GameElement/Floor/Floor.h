#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"

class Floor : public Collider {
public:
	Floor(const std::string& textureName,Vector3 position, Vector3 scale,ColliderMask ColliderMask = ColliderMask::FLOOR);
	//void Init();
	void Update();
	void Draw(const Camera* camera);

	void Move(Vector3 position);
	//当たり判定の更新
	void ColliderUpdate();
	//当たり判定
	void OnCollision(const Collider& collider)override;

private:
	//3Dモデル
	std::unique_ptr<Object> object_;

	//当たり判定発生のための
	float kColliderFlame = 0.0f;
	//当たり判定が発生するまでの時間
	const float kColliderInterval = 10.0f;
};