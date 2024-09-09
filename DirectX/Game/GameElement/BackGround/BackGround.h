#pragma once
#include "Model.h"
#include "PostEffect/PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <vector>

class BackGround {
public:
	BackGround();

	void Update(const float& deltaTime);
	void Draw(const Camera& camera);

private:
	void DrawSprites();
	void SetGlobalVariables();
	void ApplyGlobalVariables();

private://プライベート関数
	//Planeモデル
	std::unique_ptr<Model> model_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<PostEffect> postEffect2_;
	std::unique_ptr<GlobalVariableUser> global_;

	std::unique_ptr<Sprite> back_;
	std::vector<std::unique_ptr<Sprite>> verticals_;
	std::vector<std::unique_ptr<Sprite>> horizontals_;

	Vector2 basePos_;
	float size2_;
	Vector2 move_;
	Vector2 screenSize_;
	float firstPos;
};