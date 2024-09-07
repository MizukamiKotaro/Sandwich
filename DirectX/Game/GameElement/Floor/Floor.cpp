#include "Floor.h"

void Floor::Init()
{
	//板ポリに画像を貼り付ける
	object_ = std::make_unique<Object>("circle.png");
	object_->Update();
}

void Floor::Update()
{
	object_->Update();
}

void Floor::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}
