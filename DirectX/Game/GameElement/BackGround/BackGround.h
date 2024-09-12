#pragma once
#include "Model.h"
#include "PostEffect/PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <vector>
#include "GameElement/Timer/Timer.h"

class GameManager;

class BackGround {
public:
	BackGround();

	void Initialise();
	void Update(const float& deltaTime);
	void Draw(const Camera& camera);

	const bool GetIsTimeUp() const;
	const bool& GetIsReset() const;

private:
	void Create();
	void DrawSprites();
	void SetGlobalVariables();
	void ApplyGlobalVariables();

private:
	GameManager* gameManager_;

	enum SpriteNames {
		kTitle,
		kStartUI,
		kSpaceUI,
		kEnd,
	};
	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector<std::string> names_;

	struct PutData
	{
		Vector2 basePos;
		Vector2 baseScale;
		float scale;
	};
	std::vector<PutData> putDatas_;

private:
	std::unique_ptr<Timer> timer_;

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
	bool preResult_;
};