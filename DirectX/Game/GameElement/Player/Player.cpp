#include "Player.h"
#include "FrameInfo/FrameInfo.h"

void Player::Init()
{
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>("circle.png");
	//当たり判定
	CreateCollider(ColliderShape::BOX2D,ColliderType::COLLIDER,ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::FLOOR);

	input_ = Input::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character","Player");
	SetGlobalVariables();
	object_->Update();

	panTop = std::make_unique<Floor>("circle.png", Vector3{ 0.0f,topLimit ,0.0f }, Vector3{ 100.0f,0.1f,1.0f },ColliderMask::PAN);

	panBottom = std::make_unique<Floor>("circle.png", Vector3{ 0.0f,bottomLimit ,0.0f }, Vector3{ 100.0f,0.1f,1.0f });

}

void Player::Update()
{
#ifdef _DEBUG
	//ApplyGlobalVariables();
#endif

#pragma region ImGui
	ImGui::Begin("Play");
	if (ImGui::Button("clear")) {
		floor_.clear();
	}
	ImGui::End();
#pragma endregion
	//天井の判定の代わり
	if (object_->model->transform_.translate_.y > topLimit) {
		isHitCeiling = true;
	}
	if (isHitCeiling) {
		HitCeiling();
		floor_.clear();
		if (object_->model->transform_.translate_.y < bottomLimit) {

			HitBottom();
		}
		object_->Update();
		panTop->Update();
		return;
	}

	jumpFlame += FrameInfo::GetInstance()->GetDeltaTime();
	//ジャンプをしない

	jumpForceVec.y = jumpForce;
		if (input_->PressedKey(DIK_SPACE)) {
			JumpInit();
			CreateFloor();
		}
	if (jumpFlame > kJumpInterval) {
		//スペースでジャンプ初期化

	}
	//ジャンプの処理
	if (jumpFlag) {
		Jump();
	}

	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	object_->model->transform_.translate_ += jumpForceVec * deltaTime;
	jumpForce -= 0.1f;

	//移動制御
	object_->SetTranslate({ std::clamp(object_->GetWorldTransform().translate_.x, -18.0f, 18.0f) ,object_->GetWorldTransform().translate_.y,0.0f });

	//床更新
	for (std::list<std::unique_ptr<Floor>>::iterator it = floor_.begin(); it != floor_.end(); it++)
	{
		(*it)->Update();
	}

	object_->Update();

	ColliderUpdate();

	//パンの更新
	panTop->Update();
	panBottom->Update();
}

void Player::Draw(const Camera* camera)
{
	object_->Draw(*camera);

	//床描画
	for (std::list<std::unique_ptr<Floor>>::iterator it = floor_.begin(); it != floor_.end(); it++)
	{
		(*it)->Draw(camera);
	}
	//パンの描画
	if (isHitCeiling) {
		panTop->Draw(camera);
	}
	panBottom->Draw(camera);
}

void Player::JumpInit()
{
	jumpFlame = 0;
	jumpFlag = true;
	jumpForce = 10.0f;
	jumpForceVec.x = 0.0f;
	jumpForceVec.y = jumpForce;
}

void Player::Jump()
{
	jumpForceVec.x = 0.0f;
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
	ImGui::Text("%f", object_->GetWorldTransform().translate_.x);
	ImGui::End();

	jumpForceVec.Length();

}

void Player::CreateFloor()
{
	floor_.push_back(std::unique_ptr<Floor>(new Floor("cheese.png", object_->model->transform_.translate_, { 5.0f,0.1f,1.0f })));
}

void Player::HitCeiling()
{
	const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();
	dropSpeed_ = 50.0f * deltaTime;
	object_->model->transform_.translate_.y -= dropSpeed_;
	panTop->Move(object_->model->transform_.translate_ - Vector3{0.0f,1.0f,0.0f});
}

void Player::HitBottom()
{
	panTop->Move({ 0.0f,topLimit ,0.0f });
	isHitCeiling = false;

}

void Player::ColliderUpdate()
{	
	SetBox2D(object_->GetWorldTransform().translate_,object_->GetWorldTransform().scale_);
	SetCollider();
}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::FLOOR) {
		collider;
		jumpForce = 10.0f;
		jumpForceVec.x = 0.0f;
	}
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
