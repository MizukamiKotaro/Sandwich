#include "Player.h"

void Player::Init()
{
	object_ = std::make_unique<Object>("circle.png");
}

void Player::Update()
{

}

void Player::Draw(const Camera* camera)
{
	object_->Draw(*camera);
}
