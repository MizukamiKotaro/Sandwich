#pragma once
#include <iostream>
#include <algorithm>
#include "GameElement/Object/Object.h"
#include "GameElement/Floor/Floor.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Input.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "ImGuiManager/ImGuiManager.h"
#include "RandomGenerator/RandomGenerator.h"

class Player : public Collider {
public://パブリック関数
#pragma region
	void Init();
	void Update();
	void Draw(const Camera* camera);

	//trueなら落下中
	bool GetIsDrop() const { return isHitCeiling; };
	float GetIsDropSpeed()const { return dropSpeed_; };
#pragma endregion
public://パブリック変数
#pragma region 

#pragma endregion
private://プライベート関数
#pragma region

	//ジャンプ関連
	void JumpInit();
	void CommonJumpInit();
	void Jump();

	//プレイヤーのテクスチャをランダムで選択する(かぶりなし)
	int randomTextureSelect(int PreTextture);

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

	//Jump
	//ジャンプのインターバル
	float kJumpInterval = 2.0f;//2秒
	//ジャンプのフレーム
	float jumpFlame;
	//ジャンプの初期値
	float kJumpForce = 10.0f;
	//ジャンプの力
	float jumpForce;
	//横移動の強さ
	float kJumpForceX = 5.0f;
	//プレイヤーにかかる重力
	float gravity = 0.05f;

	//ジャンプの力
	Vector3 jumpForceVec;
	//ジャンプのフラグ
	bool jumpFlag = false;
	//ジャンプした時のXの移動量
	float jumpXmovement = 0.0f;
	//ジャンプした時の中心点
	float jumpXCenter = 0.0f;
	//ジャンプのテクスチャ
	std::vector<std::string> jumpTexture;
	Matrix4x4 lookLeftMatrix;
	Matrix4x4 lookRightMatrix;
	int preTexture = 0;
	int currentTexture;
	//HitCeiling
	bool isHitCeiling = false;

	float kDropSpeed = 50.0f;
	float dropSpeed_;

	const float topLimit = 20.0f;
	float panTopY;

	const float bottomLimit = -15.0f;

	//パンのインスタンス
	std::unique_ptr<Floor> panTop;
	std::unique_ptr<Floor> panBottom;
	//パンのサイズ
	Vector3 panSize = {20.0f,1.0f,1.0f};

#pragma endregion

private://基本機能たち
#pragma region
	Input* input_ = nullptr;
	std::unique_ptr<GlobalVariableUser> global;
	RandomGenerator* random;
#pragma endregion
};