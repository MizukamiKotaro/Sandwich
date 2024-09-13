#include "PostSprite.h"

PostSprite::PostSprite(const Vector2& pos)
{
	post_ = std::make_unique<PostEffect>();
	sprite_ = std::make_unique<Sprite>(pos);
	sprite_->SetSRVGPUDescriptorHandle_(post_->GetSRVGPUDescriptorHandle());
	time_ = 0.0f;
}

void PostSprite::Update()
{
	sprite_->Update();
}

void PostSprite::Draw()
{
	sprite_->Draw();
}

void PostSprite::PreDrawScene()
{
	post_->PreDrawScene();
}

void PostSprite::PostDrawScene()
{
	post_->PostDrawScene();
}
