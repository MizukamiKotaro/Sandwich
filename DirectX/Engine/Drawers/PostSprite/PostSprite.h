#pragma once
#include "PostEffect/PostEffect.h"
#include "Sprite.h"
#include <memory>

class PostSprite
{
public:
	PostSprite(const Vector2& pos);

	void Update();
	void Draw();

	void PreDrawScene();
	void PostDrawScene();

public:
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<PostEffect> post_;
};