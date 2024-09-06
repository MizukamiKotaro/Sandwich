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
	//ApplyGlobalVariables();
#endif

	//const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();

	AutoJumpSystem();
	if (jumpFlag) {
		Jump();
	}

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
	//ジャンプをする
	else {
		jumpFlame = 0;
		jumpFlag = true;
		JumpInit();
	}
}

void Player::JumpInit()
{
	jumpForce = 10.0f;
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;
}

void Player::Jump()
{

	jumpForceVec.y = jumpForce;

	if (input_->PressingKey(DIK_A)) {
		jumpForceVec.x = -5.0f;
		jumpForce -= 0.05f;
	}
	else if (input_->PressingKey(DIK_D)) {
		jumpForceVec.x = 5.0f;
		jumpForce -= 0.05f;
	}

	ImGui::Begin("Player");
	ImGui::Text("%f", object_->GetWorldTransform().translate_.y);
	ImGui::End();

	jumpForceVec.Length();

	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	object_->model->transform_.translate_ += jumpForceVec * deltaTime;
	jumpForce -= 0.1f;

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
