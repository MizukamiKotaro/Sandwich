#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Equipment : public Collider{
public:
	void Init();
	void Update();
	void Draw(const Camera* camera);

private:
	//当たり判定の更新
	void ColliderUpdate();
	//当たり判定
	void OnCollision(const Collider& collider)override;
private:

	void SetGlobalVariables();
	void ApplyGlobalVariables();

private:
	std::unique_ptr<Object> object_;
	std::unique_ptr<GlobalVariableUser> global;
};