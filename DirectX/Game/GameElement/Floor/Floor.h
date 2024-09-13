#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Ease/Ease.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Player;
class Floor : public Collider {
public:
	Floor(const std::string& textureName,Vector3 position, Vector3 scale,Player* player,ColliderMask ColliderMask = ColliderMask::FLOOR,bool isColliderUse = true);
	//void Init();
	void Update();
	void Draw(const Camera* camera);

	void SetSize(Vector3 size);
	void Move(Vector3 position);
	Vector3 GetPos() { return object_->model->transform_.translate_; };

	//3Dモデル
	std::unique_ptr<Object> object_;
private:
	//Globalvariables
	void SetGlobalVariables();
	void ApplyGlobalVariables();
	//当たり判定の更新
	void ColliderUpdate(const Vector3& move);
	//当たり判定
	void OnCollision(const Collider& collider)override;
	void StepOn();

	Player* player_;

	//当たり判定が発生するまでの時間
	const float kColliderInterval = 10.0f;
	//踏まれたかどうかのフラグ
	bool IsStepOnFlag = false;
	//0.5f超えたら
	bool IsFrameOver;
	//経過フレーム
	float stepOnFrame;
	//コライダーを使うかどうか
	bool useCollider;
	//コライダー使うかどうか
	bool isuseCollider = true;

	//踏んだ時の戻る速度
	float ratio = 2.0f;

	Vector3 prePos;
	Vector3 targetPos;

	std::unique_ptr<GlobalVariableUser> global;

	// 移動量を求めるよう
	Vector3 prePos_;
};