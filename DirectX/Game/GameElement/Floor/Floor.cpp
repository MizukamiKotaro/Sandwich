#include "Floor.h"

Floor::Floor(const std::string& textureName,Vector3 position) {
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>(textureName);

	object_->model->transform_.translate_ = position;
	object_->model->transform_.scale_ = { 100.0f,0.1f,1.0f };

	object_->Update();

	//当たり判定
	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::FLOOR);
}

void Floor::Update()
{
	object_->Update();
}

void Floor::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}

void Floor::ColliderUpdate()
{

}

void Floor::OnCollision(const Collider& collider)
{
	collider;
}
