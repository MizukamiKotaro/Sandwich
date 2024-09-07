#include "Equipment.h"
#include "FrameInfo/FrameInfo.h"

void Equipment::Init()
{
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>("circle.png");
	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::ENEMY);

	global = std::make_unique<GlobalVariableUser>("Character", "Equipment");
	SetGlobalVariables();
	object_->Update();
}

void Equipment::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif

	//const float deltaTime = FrameInfo::GetInstance()->GetDeltaTime();

	object_->Update();

	ColliderUpdate();
}

void Equipment::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}

void Equipment::ColliderUpdate()
{	
	SetBox2D(object_->GetWorldTransform().translate_,object_->GetWorldTransform().scale_);
	SetCollider();
}

void Equipment::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::ENEMY) {
		float y = collider.GetLine()->y_;
		Vector3 translate = object_->GetWorldTransform().translate_;
		translate.y = y + object_->GetWorldTransform().scale_.y;
		object_->SetTranslate(translate);
		object_->Update();
	}
}

void Equipment::SetGlobalVariables()
{
	global->AddItem("拡縮",object_->GetWorldTransform().scale_,"トランスフォーム");
	global->AddItem("回転",object_->GetWorldTransform().rotate_,"トランスフォーム");
	global->AddItem("座標",object_->GetWorldTransform().translate_,"トランスフォーム");
	ApplyGlobalVariables();
}

void Equipment::ApplyGlobalVariables()
{
	object_->SetTranslate(global->GetVector3Value("座標","トランスフォーム"));
}
