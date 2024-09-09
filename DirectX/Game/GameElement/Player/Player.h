#pragma once
#include <iostream>
#include <algorithm>
#include "GameElement/Object/Object.h"
#include "GameElement/Floor/Floor.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Input.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "ImGuiManager/ImGuiManager.h"

class Player : public Collider{
public://パブリック関数
#pragma region
	void Init();
	void Update();
	void Draw(const Camera* camera);
#pragma endregion
public://パブリック変数
#pragma region 

#pragma endregion
private://プライベート関数
#pragma region

	//ジャンプ関連
	void JumpInit();
	void Jump();

	//床関連
	//床を作る
	void CreateFloor();

	//天井に当たったら
	void HitCeiling();
	//床に当たったら
	void HitBottom();

	//当たり判定の更新
	void ColliderUpdate();
	//当たった時の処理
	void OnCollision(const Collider& collider)override;

	//Globalvariables
	void SetGlobalVariables();
	void ApplyGlobalVariables();
#pragma endregion
private://プライベート変数
#pragma region
	//3Dモデル
	std::unique_ptr<Object> object_;

	//床をまとめた物
	std::list<std::unique_ptr<Floor>> floor_;

	//AutoJump
	//ジャンプのインターバル
	const float kJumpInterval = 2.0f;//2秒
	//ジャンプのフレーム
	float jumpFlame;
	//ジャンプの力
	float jumpForce;
	//ジャンプの力
	Vector3 jumpForceVec;
	//ジャンプのフラグ
	bool jumpFlag = false;

	//HitCeiling
	bool isHitCeiling = false;

	const float topLimit = 20.0f;
	float panTopY;

	const float bottomLimit = -15.0f;

	std::unique_ptr<Floor> panTop;
	std::unique_ptr<Floor> panBottom;

#pragma endregion

private://基本機能たち
#pragma region
	Input* input_ = nullptr;
	std::unique_ptr<GlobalVariableUser> global;
#pragma endregion
};