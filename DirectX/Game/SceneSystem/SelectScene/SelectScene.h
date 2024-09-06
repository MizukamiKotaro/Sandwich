#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include <array>

class SelectScene : public IScene
{
public:
	SelectScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:


	std::array<std::unique_ptr<Sprite>, COUNT_STAGE> musicSprites_;

	std::unique_ptr<Sprite> space_;

	std::unique_ptr<Audio> se_;
};

