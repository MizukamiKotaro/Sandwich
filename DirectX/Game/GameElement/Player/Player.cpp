#include "Player.h"
#include "FrameInfo/FrameInfo.h"



void Player::Init()
{
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>("circle.png");
	//当たり判定
	CreateCollider(ColliderShape::BOX2D,ColliderType::COLLIDER,ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::ENEMY);

	input_ = Input::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character","Player");
	SetGlobalVariables();
	object_->Update();
}

void Player::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif

	//const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();

	object_->Update();

	ColliderUpdate();
}

void Player::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}

void Player::Move()
{
	//if (input_->PressedKey(DIK_A)) {

	//}
	//else if (input_->PressedKey(DIK_D)) {

	//}
}

void Player::AutoJumpSystem()
{
	jumpFlame += FrameInfo::GetInstance()->GetDeltaTime();
	//ジャンプをしない
	if (jumpFlame < kJumpInterval) {

	}
	else {

	}
}

void Player::JumpInit()
{
	jumpForce = 1.0f;
}

void Player::Jump()
{
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	object_->model->transform_.translate_.y -= jumpForce * deltaTime;
}

void Player::ColliderUpdate()
{	
	SetBox2D(object_->GetWorldTransform().translate_,object_->GetWorldTransform().scale_);
	SetCollider();
}

void Player::OnCollision(const Collider& collider)
{
	collider;
}

void Player::SetGlobalVariables()
{
	global->AddItem("拡縮",object_->GetWorldTransform().scale_,"トランスフォーム");
	global->AddItem("回転",object_->GetWorldTransform().rotate_,"トランスフォーム");
	global->AddItem("座標",object_->GetWorldTransform().translate_,"トランスフォーム");
	ApplyGlobalVariables();
}

void Player::ApplyGlobalVariables()
{
	object_->SetTranslate(global->GetVector3Value("座標","トランスフォーム"));
}
