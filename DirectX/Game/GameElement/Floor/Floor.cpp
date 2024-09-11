#include "Floor.h"
#include "GameElement/Player/Player.h"
#include "FrameInfo/FrameInfo.h"

Floor::Floor(const std::string& textureName, Vector3 position, Vector3 scale, Player* player, ColliderMask ColliderMask) {
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(textureName);

	object_->model->transform_.translate_ = position;
	object_->model->transform_.scale_ = scale;

	object_->Update();

	player_ = player;

	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask);
	AddTargetMask(ColliderMask::PAN);
	AddTargetMask(ColliderMask::PLAYER);
}

void Floor::Update()
{
	if (IsStepOnFlag) {
		StepOn();
	}


	ColliderUpdate();

	object_->Update();
}

void Floor::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}

void Floor::SetSize(Vector3 size)
{
	object_->model->transform_.scale_ = size;
}

void Floor::Move(Vector3 position)
{
	object_->model->transform_.translate_ = position;
}

void Floor::ColliderUpdate()
{
	SetBox2D(object_->GetWorldTransform().translate_, object_->GetWorldTransform().scale_);
	SetCollider();
}

void Floor::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PAN) {
		object_->model->transform_.translate_.y = player_->GetTlanslate().y - 2.0f;
	}
	if (collider.GetMask() == ColliderMask::PLAYER) {
		if (GetMask() == ColliderMask::PAN) { return; };
		IsStepOnFlag = true;
	}
}

void Floor::StepOn(){
	stepOnrlame += FrameInfo::GetInstance()->GetDeltaTime();

	//Ease::UseEase(object_->model->transform_.translate_, {0.0f,0.0f,0.0f}, Ease::EaseInBack);

	if (stepOnrlame >= 1.0f) {
		IsStepOnFlag = false;
		stepOnrlame = 0.0f;
	}
}
