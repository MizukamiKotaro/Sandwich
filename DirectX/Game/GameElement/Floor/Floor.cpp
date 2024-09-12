#include "Floor.h"
#include "GameElement/Player/Player.h"
#include "FrameInfo/FrameInfo.h"

Floor::Floor(const std::string& textureName, Vector3 position, Vector3 scale, Player* player, ColliderMask ColliderMask, bool isColliderUse) {
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(textureName);

	object_->model->transform_.translate_ = position;
	object_->model->transform_.scale_ = scale;

	object_->Update();

	player_ = player;

	useCollider = isColliderUse;
	//当たり判定
	if (useCollider) {
		CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask);
		AddTargetMask(ColliderMask::PAN);
		AddTargetMask(ColliderMask::PLAYER);
	}

	global = std::make_unique<GlobalVariableUser>("Character", "cheese");

	SetGlobalVariables();
}

void Floor::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif

	if (IsStepOnFlag) {
		StepOn();
	}

	if (useCollider) {
		ColliderUpdate();
	}

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

void Floor::SetGlobalVariables()
{

	global->AddItem("速度の倍率", ratio,"踏みつけ");

	ApplyGlobalVariables();
}

void Floor::ApplyGlobalVariables()
{
	ratio = global->GetFloatValue("速度の倍率", "踏みつけ");
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
		if (GetMask() == ColliderMask::PREDICTIONLINE) { return; };
		prePos = object_->model->transform_.translate_;
		targetPos = { prePos.x,prePos.y - 0.5f,prePos.z };
		IsFrameOver = false;
		IsStepOnFlag = true;
	}
}

void Floor::StepOn() {



	stepOnFrame += FrameInfo::GetInstance()->GetDeltaTime() * ratio;

	stepOnFrame = std::min(stepOnFrame,1.0f);

	if (IsFrameOver == false) {
		object_->model->transform_.translate_ = Ease::UseEase(prePos, targetPos, stepOnFrame,Ease::EaseIn);
	}
	else {
		object_->model->transform_.translate_ = Ease::UseEase(targetPos, prePos, stepOnFrame, Ease::EaseIn);
	}

	if (stepOnFrame >= 1.0f && IsFrameOver) {
		IsStepOnFlag = false;
		stepOnFrame = 0.0f;
	}
	if (IsFrameOver == false && stepOnFrame >= 1.0f) {
		IsFrameOver = true;
		stepOnFrame = 0.0f;
	}

}
