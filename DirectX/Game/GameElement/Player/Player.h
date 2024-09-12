#pragma once
#include <iostream>
#include <algorithm>
#include "GameElement/Object/Object.h"
#include "GameElement/Floor/Floor.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Input.h"
#include "GlobalVariables/GlobalVariableUser.h"

#include "RandomGenerator/RandomGenerator.h"
#include "Audio/Audio.h"

class Player : public Collider {
public://パブリック関数
#pragma region
	void Init();
	void Update();
	void Draw(const Camera* camera);

	Vector3 GetTlanslate() { return object_->model->transform_.translate_; };
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
	void ColliderUpdate(const Vector3& move);
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
	std::list<std::unique_ptr<Floor>> cheese_;
	std::unique_ptr<Floor> predictionLine;
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
	float gravity = 0.1f;
	//ボタンを押したときにかかる力
	float buttomGravity = 0.05f;
	//下ボタンを押したときにかかる力
	float buttomDown = 0.05f;
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
	//左右を反転させるための行列
	Matrix4x4 lookLeftMatrix;
	Matrix4x4 lookRightMatrix;
	//左右の移動制限
	float Xlimit = 18.0f;
	//ジャンプのSE
	std::unique_ptr<Audio> jumpSE;
	//ひとつ前で選択されたテクスチャ
	int preTexture = 0;
	//現在のテクスチャ
	int currentTexture;
	//天井に当たったかのフラグ
	bool isHitCeiling = false;
	//下のパンを描画するかどうかのフラグ
	bool isDrawbottomPanFlag = true;
	//落下速度
	float kDropSpeed = 50.0f;
	float dropSpeed_;
	//上側のパンの位置(Y)
	float topLimit = 20.0f;
	float panTopY;
	//下側のパンの位置(Y)
	float bottomLimit = -12.0f;
	//下のパン出現条件
	float bottomPanReset = -8.0f;
	//どこまで落下するか
	float bottomDropLimit = -15.0f;
	//プレイヤーをパンの上に戻す
	bool isPlayerBackFlag = false;

	bool panbottomMove = false;
	
	bool player;

	float prePanPos;

	//パンのインスタンス
	std::unique_ptr<Floor> panTop;
	std::unique_ptr<Floor> panBottom;
	//パンのサイズ
	Vector3 panSize = { 20.0f,1.0f,1.0f };
	//上のパンを描画
#ifdef _DEBUG
	bool IsDraw = false;
#endif

#pragma endregion

private://基本機能たち
#pragma region
	Input* input_ = nullptr;
	std::unique_ptr<GlobalVariableUser> global;
	RandomGenerator* random;
#pragma endregion
};