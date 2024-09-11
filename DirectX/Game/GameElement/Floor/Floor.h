#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Ease/Ease.h"

class Player;
class Floor : public Collider {
public:
	Floor(const std::string& textureName,Vector3 position, Vector3 scale,Player* player,ColliderMask ColliderMask = ColliderMask::FLOOR);
	//void Init();
	void Update();
	void Draw(const Camera* camera);

	void SetSize(Vector3 size);
	void Move(Vector3 position);

	//3Dモデル
	std::unique_ptr<Object> object_;
private:
	//当たり判定の更新
	void ColliderUpdate();
	//当たり判定
	void OnCollision(const Collider& collider)override;
	void StepOn();

	Player* player_;

	//当たり判定が発生するまでの時間
	const float kColliderInterval = 10.0f;
	//踏まれたかどうかのフラグ
	bool IsStepOnFlag = false;
	//経過フレーム
	float stepOnrlame;

};