#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"

class ClearScene : public IScene
{
public:
	ClearScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	
	bool isEnd_;

	std::unique_ptr<Sprite> space_;
	
	Audio se_;
};
