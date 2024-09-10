#include "Floor.h"

Floor::Floor(const std::string& textureName,Vector3 position,Vector3 scale,ColliderMask ColliderMask) {
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(textureName);

	object_->model->transform_.translate_ = position;
	object_->model->transform_.scale_ = scale;

	object_->Update();

	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask);

}

void Floor::Update()
{

	ColliderUpdate();
	
	object_->Update();

}

void Floor::Draw(const Camera* camera)
{
	object_->Draw(*camera);
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
	collider;
}
